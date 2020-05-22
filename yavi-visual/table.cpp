#include "func.h"
#include "goods.h"
#include "table.h"

//------------------------------------------------------------------------------

TTable::TTable( TGoods  *pAncestor )
{
    m_zId.clear();
    m_zName.clear();
    m_zLink.clear();

    // указатель на родителя
    m_pAncestor = pAncestor;

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button", this );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    m_grid->addWidget( m_ptBtnName, m_row, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_grid->addWidget( m_ptBtnInc, m_row, 1, Qt::AlignLeft );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    this->setLayout( m_grid );
}

TTable::~TTable()
{

}

//------------------------------------------------------------------------------

void  TTable::onBtnName()
{
    qDebug() << getTableName() << "button";
}

void  TTable::onBtnInc()
{
    qDebug() << "Inc button" << getTableId() << getTableName();
}

//------------------------------------------------------------------------------

void  TTable::TableDelete()
{
    QLayoutItem *child;

    // уничтожаем диалог
//    m_ptDialog->~TDialog();

//    qDebug() << getTableName() << "del dialog";

    // уничтожаем виджеты
    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

//    qDebug() << getTableName() << "del widgets";

    // уничтожаем grid
    m_grid->deleteLater();

//    qDebug() << getTableName() << "del grid";

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
    }
}

void  TTable::setTableId( const std::string&  name )
{
    m_zId = QString::fromStdString(name);
}

const QString TTable::getTableId()
{
    return m_zId;
}

void  TTable::setTableName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );  // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );             // правленное имя кнопки
    m_ptBtnName->setToolTip( m_zName );             // подсказка с оригинальным именем
}

const QString TTable::getTableName()
{
    return m_zName;
}

void  TTable::setTableLink( const std::string&  name )
{
    m_zLink = QString::fromStdString(name);

    m_ptLblLink = new QLabel( this, Q_NULLPTR );
    m_ptLblLink->setText( m_zLink );
//    m_ptLblLink->setMinimumSize(95,25);
    m_ptLblLink->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblLink, m_row, 1, 1, -1, Qt::AlignLeft );

    nextRow();
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
        QLabel  *label = new QLabel( this, Q_NULLPTR );
        label->setText( it );
        label->setMinimumWidth( 93 );
        label->setAlignment( Qt::AlignCenter );
        label->setFrameStyle( QFrame::Panel | QFrame::Raised );
        m_grid->addWidget( label, m_row, m_column + column, Qt::AlignLeft );

        column += 1;
    }

    nextRow();
}

void TTable::setTableColumn( QStringList& list )
{
    int row = 0;

    for( auto& it : list )
    {
        QLabel  *label = new QLabel( this, Q_NULLPTR );
        label->setText( it );
        label->setMinimumWidth( 93 );
        label->setAlignment( Qt::AlignCenter );
        label->setFrameStyle( QFrame::Panel | QFrame::Raised );
        m_grid->addWidget( label, m_row + row, m_column, Qt::AlignLeft );

        row += 1;
    }

    nextColumn();
}

void TTable::resetRow()
{
    m_row = 0;
}

void TTable::resetColumn()
{
    m_column = 0;
}


void TTable::nextRow()
{
    m_row += 1;
}

void TTable::nextColumn()
{
    m_column += 1;
}

int TTable::getTableWidth()
{
    QSize size = m_grid->minimumSize();

    return size.width();
}

int TTable::getTableHeight()
{
    QSize size = m_grid->minimumSize();

    return size.height();
}

//------------------------------------------------------------------------------
