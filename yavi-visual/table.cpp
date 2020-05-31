#include "func.h"
#include "goods.h"
#include "table.h"

//------------------------------------------------------------------------------

TTable::TTable( TGoods  *pAncestor )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialog, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    // указатель на родителя
    m_pAncestor = pAncestor;

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_grid->setMargin(0);

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    m_grid->addWidget( m_ptBtnName, m_row, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить запись" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_grid->addWidget( m_ptBtnInc, m_row, 1, Qt::AlignLeft );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    setLayout( m_grid );

    widget_size_reset();

    widget_stretch( m_grid->minimumSize().width(), m_grid->minimumSize().height() );
}

TTable::~TTable()
{

}

void  TTable::clear()
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_uTableType = keTypeNone;

    m_zId.clear();
    m_zName.clear();
    m_zLink.clear();

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TTable::onBtnName()
{
    //qDebug() << getTableName() << "button";

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgEnabled( false );

    m_ptDialog->setDlgName( getTableName() );

    m_ptDialog->open();
}

void  TTable::onBtnInc()
{
    //qDebug() << "Inc button" << getTableId() << getTableName();

    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgName( "NewRecord" );

    m_ptDialog->open();
}

void  TTable::onSendCancel()
{
    need_to_add = false;
}

void  TTable::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    if( false == need_to_add )
    {
        // редактируем текущий набор параметров

        setTableName( m_tValues.m_zName.toStdString(), true );

        //qDebug() << getCategoriesName() << "fix categories";
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров

    }

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TTable::TableDelete()
{
    QLayoutItem *child;

    // уничтожаем виджеты
    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем grid
    m_grid->deleteLater();

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pAncestor )
    {
        for( int i = 0; i < m_pAncestor->m_apTableList.count(); i++ )
        {
            if( this == m_pAncestor->m_apTableList.at(i) )
            {
                //qDebug() << m_pAncestor->m_apTableList.at(i)->getTableName() << "obsolete";

                m_pAncestor->m_apTableList.removeAt(i);
            }
        }

        // удаляемся из родительского layout-а
        m_pAncestor->m_vlayout->removeWidget(this);
    }
}

//------------------------------------------------------------------------------

void  TTable::setTableType( unsigned type ) noexcept
{
    m_uTableType = type;

    //qDebug() << "table" << getTableName() << "type" << m_uTableType;
}

//------------------------------------------------------------------------------

void  TTable::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTable::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TTable::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTable::getNode()
{
    return m_node;
}

YAML::Node&  TTable::getNodeParent()
{
    return m_node_parent;
}

int  TTable::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TTable::setTableId( const std::string&  name, bool  set_to_node )
{
    m_zId = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsTableId, name );
    }
}

const QString TTable::getTableId()
{
    return m_zId;
}

void  TTable::setTableName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Таблица: " + m_zName );    // подсказка с оригинальным именем

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsTableName, name );
    }
}

const QString TTable::getTableName()
{
    return m_zName;
}

void  TTable::setTableLink( const std::string&  name, bool  set_to_node )
{
    m_zLink = QString::fromStdString(name);

    m_ptLblLink = new QLabel( this, Q_NULLPTR );
    m_ptLblLink->setText( m_zLink );
//    m_ptLblLink->setMinimumSize(95,25);
    m_ptLblLink->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblLink, m_row, 1, 1, -1, Qt::AlignLeft );

    widget_stretch( m_grid->minimumSize().width(), m_grid->minimumSize().height() );

    nextRow();

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsTableLink, name );
    }
}

const QString TTable::getTableLink()
{
    return m_zLink;
}

void TTable::setTableRow( QStringList& list )
{
    int column = 0;

    for( auto& it : list )
    {
        QLabel  *label = new QLabel();
        label->setText( it );
        label->setFixedWidth( 93 );
        label->setAlignment( Qt::AlignCenter );
        label->setFrameStyle( QFrame::Panel | QFrame::Raised );
        m_grid->addWidget( label, m_row, m_column + column, Qt::AlignLeft );

        widget_stretch( m_grid->minimumSize().width(), label->minimumSize().height() + m_grid->spacing() );

        column += 1;
    }

    nextRow();
}

void TTable::setTableColumn( QStringList& list )
{
    int row = 0;

    for( auto& it : list )
    {
        QLabel  *label = new QLabel();
        label->setText( it );
        label->setFixedWidth( 93 );
        label->setAlignment( Qt::AlignCenter );
        label->setFrameStyle( QFrame::Panel | QFrame::Raised );
        m_grid->addWidget( label, m_row + row, m_column, Qt::AlignLeft );

        widget_stretch( m_grid->minimumSize().width(), label->minimumSize().height() + m_grid->spacing() );

        row += 1;
    }

    nextColumn();
}

void TTable::resetRow() noexcept
{
    m_row = 0;
}

void TTable::resetColumn() noexcept
{
    m_column = 0;
}


void TTable::nextRow() noexcept
{
    m_row += 1;
}

void TTable::nextColumn() noexcept
{
    m_column += 1;
}

//------------------------------------------------------------------------------

void  TTable::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;
}

void  TTable::widget_stretch( int width, int height, bool add_height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    //qDebug() << "table" << getTableName() << m_width << m_height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_stretch( width, height, add_height );
}

void  TTable::widget_parent_stretch( int width, int height, bool add_height ) noexcept
{
    int  val = 0;

    if( Q_NULLPTR != m_pAncestor )
    {
        if( add_height )
        {
            val = m_pAncestor->m_vlayout->spacing();
        }

        m_pAncestor->widget_stretch( width, height + val );
    }
}

void  TTable::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_shrink( width, height );
}

void  TTable::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_shrink( width, height );
    }
}

int TTable::getTableWidth() noexcept
{
    return minimumSize().width();
}

int TTable::getTableHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------
