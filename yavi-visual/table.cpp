#include "table.h"

//------------------------------------------------------------------------------

TTable::TTable()
{
    m_zId.clear();
    m_zName.clear();
    m_zLink.clear();

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-", this );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    m_grid->addWidget( m_ptBtnDec, m_row, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setMinimumWidth( 94 );
    m_ptLblName->setAlignment( Qt::AlignCenter );
    m_ptLblName->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblName, m_row, 1, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_grid->addWidget( m_ptBtnInc, m_row, 2, Qt::AlignLeft );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    this->setLayout( m_grid );
}

TTable::~TTable()
{
    QLayoutItem *child;

    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }
}

//------------------------------------------------------------------------------

void  TTable::onBtnDec()
{
    qDebug() << "Dec button" << getTableId() << getTableName();
}

void  TTable::onBtnInc()
{
    qDebug() << "Inc button" << getTableId() << getTableName();
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
    m_ptLblName->setText( m_zName );
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
        label->setMinimumWidth( 94 );
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
        label->setMinimumWidth( 94 );
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
