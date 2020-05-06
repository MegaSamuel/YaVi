#include "category.h"

TCategory::TCategory()
{
    m_zName.clear();

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

TCategory::~TCategory()
{
    QLayoutItem *child;

    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }
}

//------------------------------------------------------------------------------

void  TCategory::onBtnDec()
{
    qDebug() << "Dec button";
}

void  TCategory::onBtnInc()
{
    qDebug() << "Inc button";
}

//------------------------------------------------------------------------------

void  TCategory::setCategoryId( const std::string&  name )
{
    m_zId = QString::fromStdString(name);
}

const QString TCategory::getCategoryId()
{
    return m_zId;
}

void  TCategory::setCategoryName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);
    m_ptLblName->setText( m_zName );
}

const QString TCategory::getCategoryName()
{
    return m_zName;
}

//------------------------------------------------------------------------------

void TCategory::resetRow()
{
    m_row = 0;
}

void TCategory::resetColumn()
{
    m_column = 0;
}


void TCategory::nextRow()
{
    m_row += 1;
}

void TCategory::nextColumn()
{
    m_column += 1;
}

int TCategory::getCategoryWidth()
{
    QSize size = m_grid->minimumSize();

    return size.width();
}

int TCategory::getCategoryHeight()
{
    QSize size = m_grid->minimumSize();

    return size.height();
}

//------------------------------------------------------------------------------
