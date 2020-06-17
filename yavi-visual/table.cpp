#include "func.h"
#include "goods.h"
#include "table.h"
#include "mainwindow.h"

//------------------------------------------------------------------------------

TTable::TTable( TGoods  *pAncestor )
{
    clear();

    // диалог для айди
    m_ptTabDialogId = new TTabDialog( false, "Table", this, "ID" );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogId, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogId, &TTabDialog::sendValue, this, &TTable::onSendValue );

    // диалог для имени
    m_ptTabDialogName = new TTabDialog( false, "Table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogName, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogName, &TTabDialog::sendValue, this, &TTable::onSendValue );

    // диалог для новой таблицы
    m_ptTabDialogAdd = new TTabDialog( true, "Add table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogAdd, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogAdd, &TTabDialog::sendValues, this, &TTable::onSendValues );

    // указатель на родителя
    m_pAncestor = pAncestor;

    // цепляем местный сигнал к слоту MainWindow
    connect( this, &TTable::sendChanged, MainWindow::getMainWinPtr(), &MainWindow::onYamlChanged );

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_grid->setMargin(0);

    // кнопка с id
    m_ptBtnId = new QPushButton( "id" );
    m_ptBtnId->setToolTip( "ID таблицы" );
    //m_ptBtnId->setFixedWidth( 93 );
    connect( m_ptBtnId, &QPushButton::clicked, this, &TTable::onBtnId );
    m_grid->addWidget( m_ptBtnId, m_row, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "name" );
    m_ptBtnName->setToolTip( "Название таблицы" );
    //m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, &QPushButton::clicked, this, &TTable::onBtnName );
    m_grid->addWidget( m_ptBtnName, m_row, 1, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить таблицу" );
    //m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, &QPushButton::clicked, this, &TTable::onBtnInc );
    m_grid->addWidget( m_ptBtnInc, m_row+1, 0, Qt::AlignLeft );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    setLayout( m_grid );

    widget_size_reset();

    int  width = 2*m_grid->margin() + m_ptBtnId->minimumSize().width() + m_ptBtnName->minimumSize().width() + m_grid->spacing();
    int  height = 2*m_grid->margin() + m_ptBtnId->minimumSizeHint().height() + m_ptBtnInc->minimumSizeHint().height() + m_grid->spacing();

    widget_stretch( width, height );
}

TTable::~TTable()
{

}

void  TTable::clear()
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_temporary_node = YAML::Node();

    m_uTableType = keTypeNone;

    m_zId.clear();
    m_zName.clear();
    m_zLink.clear();

    need_to_add = false;

    edit_id = false;
    edit_name = false;
}

//------------------------------------------------------------------------------

void  TTable::onBtnId()
{
    // признак что хотим отредактировать имя
    edit_id = true;

    // диалог с пустыми параметрами
    m_ptTabDialogId->setDlgEmpty();

    m_ptTabDialogId->setDlgName( getTableId() );

    m_ptTabDialogId->open();
}

void  TTable::onBtnName()
{
    // признак что хотим отредактировать имя
    edit_name = true;

    // диалог с пустыми параметрами
    m_ptTabDialogName->setDlgEmpty();

    m_ptTabDialogName->setDlgName( getTableName() );

    m_ptTabDialogName->open();
}

void  TTable::onBtnDec()
{
    widget_shrink( getTableWidth(), getTableHeight() + m_grid->spacing() );

    TableDelete();
}

void  TTable::onBtnInc()
{
    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptTabDialogAdd->setDlgEmpty();

    m_ptTabDialogAdd->setDlgId( "Id" );
    m_ptTabDialogAdd->setDlgName( "Table" );

    m_ptTabDialogAdd->open();
}

void  TTable::onSendCancel()
{
    need_to_add = false;

    edit_id = false;
    edit_name = false;
}

void  TTable::onSendValue( QString& a_zValue )
{
    // редактируем текущую таблицу

    if( edit_id )
    {
        m_tValues.m_zId = a_zValue;

        setTableId( m_tValues.m_zId.toStdString(), true );
    }

    if( edit_name )
    {
        m_tValues.m_zName = a_zValue;

        setTableName( m_tValues.m_zName.toStdString(), true );

        if( 0 == m_tValues.m_zName.length() )
        {
            // если имя пустое, то удаляем таблицу
            onBtnDec();
        }
    }

    edit_id = false;
    edit_name = false;
}

void  TTable::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    if( false == need_to_add )
    {
        qDebug() << "it has gone wrong!";
    }

    if( true == need_to_add )
    {
        // создаем новую таблицу

        if( 0 == m_tValues.m_zName.length() )
        {
            // если имя пустое, то ставим дефолтное имя
            m_tValues.m_zName = "noname";
        }

        // добавляем таблицу
        TTable  *pTable;
        pTable = new TTable( m_pAncestor );

        // добавляемся к родителю
        m_pAncestor->m_vlayout->addWidget( pTable );
        m_pAncestor->m_apTableList.append(pTable);

        // ставим значения параметров
        pTable->setTableId( m_tValues.m_zId.toStdString() );
        pTable->setTableName( m_tValues.m_zName.toStdString() );

        // очищаем ямл
        m_temporary_node.reset();

        // пишем их в пустой ямл
        __yaml_SetString( m_temporary_node, GoodsTableId, m_tValues.m_zId.toStdString() );
        __yaml_SetString( m_temporary_node, GoodsTableName, m_tValues.m_zName.toStdString() );

        // добавляем ямл к основному
        m_node_parent.push_back( m_temporary_node );

        int index = static_cast<int>(m_node_parent.size()) - 1;

        pTable->setNode( m_node_parent[index] );
        pTable->setNodeParent( m_node_parent );
        pTable->setNodeIndex( index );

        qDebug() << pTable->getTableName() << "index" << index;
    }

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TTable::clearNodeSequence()
{
    // если не знаем индекс, то удаляем поля
    if( -1 == m_node_index )
    {
        m_node.remove( GoodsTableId );
        m_node.remove( GoodsTableName );
        m_node.remove( GoodsTableValue );
        m_node.remove( GoodsTableColumn );
        m_node.remove( GoodsTableRow );
        m_node.remove( GoodsTableLink );
    }

    // если знаем индекс, то удаляем всю ветку
    if( -1 != m_node_index )
    {
        m_node_parent.remove( m_node_index );
    }
}

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

                break;
            }
        }

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_pAncestor->m_apTableList.count(); i++ )
        {
            index = m_pAncestor->m_apTableList.at(i)->getNodeIndex();

            if( index > m_node_index )
            {
                index -= 1;

                m_pAncestor->m_apTableList.at(i)->setNodeIndex( index );
            }
        }

        // удаляемся из родительского layout-а
        m_pAncestor->m_vlayout->removeWidget(this);
    }

    // очищаем ветку
    clearNodeSequence();
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
    int  height;

    m_zId = QString::fromStdString(name);

    height = m_ptBtnId->minimumSizeHint().height();

    m_zBtnId = QString::fromStdString(name);
    m_zBtnId.replace( QRegExp("[ ]{2,}"), " " );     // убираем подряд идущие пробелы на один
    m_zBtnId.replace( " ", "\n" );                   // заменяем пробелы на перевод строки
    m_ptBtnId->setText( m_zBtnId );                  // правленное имя кнопки
    m_ptBtnId->setToolTip( "Таблица: " + m_zId );    // подсказка с оригинальным именем

    height = m_ptBtnId->minimumSizeHint().height() - height;

    if( 0 < height )
    {
        widget_stretch( 0, height, false );
    }
    else
    {
        widget_shrink( 0, -1 * height );
    }

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
    int  height;

    m_zName = QString::fromStdString(name);

    height = m_ptBtnName->minimumSizeHint().height();

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Таблица: " + m_zName );    // подсказка с оригинальным именем

    height = m_ptBtnName->minimumSizeHint().height() - height;

    if( 0 < height )
    {
        widget_stretch( 0, height, false );
    }
    else
    {
        widget_shrink( 0, -1 * height );
    }

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

    //qDebug() << "stretch" << width << height;

    // высоту увеличиваем на каждый элемент
    m_height += height;

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

    //qDebug() << "shrink" << width << height;

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
