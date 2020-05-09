#include "param.h"

//------------------------------------------------------------------------------

TParam::TParam()
{
    clear();

    resetRow();
    resetColumn();

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-", this );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setMinimumWidth( 94 );
    m_ptLblName->setAlignment( Qt::AlignCenter );
    m_ptLblName->setFrameStyle( QFrame::Panel | QFrame::Raised );
    hlayout->addWidget( m_ptLblName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );
    // перевод строки и колонки в grid для следующих добавлений
//    nextRow();
//    nextColumn();

    this->setLayout( m_vlayout );
}

TParam::~TParam()
{
    QLayoutItem *child;

    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }
}

void  TParam::clear()
{
    m_zName.clear();
    m_zPlaceholder.clear();
    m_zNew.clear();
    m_zAfter.clear();
    m_zBefore.clear();
    m_zUlink.clear();
    m_zUname.clear();
    m_zMulti.clear();

    m_uType = 0;
    m_uMin = 0;
    m_uMax = 0;
}

//------------------------------------------------------------------------------

void  TParam::onBtnDec()
{
    qDebug() << m_zName << "dec button";
}

void  TParam::onBtnInc()
{
    qDebug() << m_zName << "inc button";
}

//------------------------------------------------------------------------------

void  TParam::setParamName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);
    m_ptLblName->setText( m_zName );
}

void  TParam::setParamPlaceholder( const std::string&  name )
{
    m_zPlaceholder = QString::fromStdString(name);
}

void  TParam::setParamNew( const std::string&  name )
{
    m_zNew = QString::fromStdString(name);
}

void  TParam::setParamAfter( const std::string&  name )
{
    m_zAfter = QString::fromStdString(name);
}

void  TParam::setParamBefore( const std::string&  name )
{
    m_zBefore = QString::fromStdString(name);
}

void  TParam::setParamUlink( const std::string&  name )
{
    m_zUlink = QString::fromStdString(name);
}

void  TParam::setParamUname( const std::string&  name )
{
    m_zUname = QString::fromStdString(name);
}

void  TParam::setParamMulti( const std::string&  name )
{
    m_zMulti = QString::fromStdString(name);
}


void  TParam::setParamType( unsigned  val )
{
    m_uType = val;
}

void  TParam::setParamMin( unsigned  val )
{
    m_uMin = val;
}

void  TParam::setParamMax( unsigned  val )
{
    m_uMax = val;
}

//------------------------------------------------------------------------------

QString  TParam::getParamName()
{
    return m_zName;
}

QString  TParam::getParamPlaceholder()
{
    return m_zPlaceholder;
}

QString  TParam::getParamNew()
{
    return m_zNew;
}

QString  TParam::getParamAfter()
{
    return m_zAfter;
}

QString  TParam::getParamBefore()
{
    return m_zBefore;
}

QString  TParam::getParamUlink()
{
    return m_zUlink;
}

QString  TParam::getParamUname()
{
    return m_zUname;
}

QString  TParam::getParamMulti()
{
    return m_zMulti;
}


unsigned  TParam::getParamType()
{
    return m_uType;
}

unsigned  TParam::getParamMin()
{
    return m_uMin;
}

unsigned  TParam::getParamMax()
{
    return m_uMax;
}

//------------------------------------------------------------------------------

void TParam::resetRow()
{
    m_row = 0;
}

void TParam::resetColumn()
{
    m_column = 0;
}

void TParam::nextRow()
{
    m_row += 1;
}

void TParam::nextColumn()
{
    m_column += 1;
}

int TParam::getParamWidth()
{
    QSize size = m_vlayout->minimumSize();

    return size.width();
}

int TParam::getParamHeight()
{
    QSize size = m_vlayout->minimumSize();

    return size.height();
}
//------------------------------------------------------------------------------
