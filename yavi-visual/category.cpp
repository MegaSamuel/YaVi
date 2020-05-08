#include "func.h"
#include "goods.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategory::TCategory()
{
    m_zName.clear();

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setMinimumWidth( 94 );
    m_ptLblName->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblName, m_row, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_grid->addWidget( m_ptBtnInc, m_row, 1, Qt::AlignLeft );

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

void  TCategory::onBtnInc()
{
    qDebug() << m_zName << "inc button";
}

//------------------------------------------------------------------------------

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
