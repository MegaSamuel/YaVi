#include "param.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategories::TCategories( TParam  *pMentor, int  depth )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( false, "Categories",  this );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    m_pMentor = pMentor;

    m_depth = depth + 1;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth; i++ )
    {
        QLabel *label = new QLabel( this, Q_NULLPTR );
        label->setMinimumWidth( 93 );
        hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-", this );
    m_ptBtnDec->setToolTip( "Удалить параметр" );
    m_ptBtnDec->setFixedWidth( 93 );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button", this );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );

    this->setLayout( m_vlayout );
}

TCategories::~TCategories()
{
    /*
    QLayoutItem *child;

    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    for( auto& it : m_apParamList )
    {
        it->~TParam();
    }
    */

//    clear();
}

void TCategories::clear()
{
    m_zName.clear();
    m_zBtnName.clear();

    m_apParamList.clear();
}

//------------------------------------------------------------------------------

void  TCategories::onBtnDec()
{
    qDebug() << getCategoriesName() << "dec button";

    CategoriesDelete();
}

void  TCategories::onBtnInc()
{
    qDebug() << getCategoriesName() << "inc button";
}

void  TCategories::onBtnName()
{
    qDebug() << getCategoriesName() << "button";

    m_ptDialog->setDlgName( getCategoriesName() );

    m_ptDialog->open();
}

void  TCategories::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    setCategoriesName( m_tValues.m_zName.toStdString() );
}

//------------------------------------------------------------------------------

void  TCategories::CategoriesDelete()
{
    QLayoutItem *child;

    // уничтожаем диалог
    m_ptDialog->~TDialog();

//    qDebug() << getCategoriesName() << "del dialog";

    // для всех вложенных Parameters вызываем очистку
    for( auto& it : m_apParamList )
    {
//        qDebug() << getCategoriesName() << "del param" << it->getParamName();

        it->ParamDelete();

        it->~TParam();
    }

//    qDebug() << getCategoriesName() << "del param";

    // уничтожаем виджеты
    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

//    qDebug() << getCategoriesName() << "del widgets";

    // уничтожаем layout
    m_vlayout->deleteLater();

//    qDebug() << getCategoriesName() << "del layout";

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pMentor )
    {
        for( int i = 0; i < m_pMentor->m_apCategoriesList.count(); i++ )
        {
            if( this == m_pMentor->m_apCategoriesList.at(i) )
            {
                qDebug() << m_pMentor->m_apCategoriesList.at(i)->getCategoriesName() << "obsolete";

                m_pMentor->m_apCategoriesList.removeAt(i);
            }
        }
    }
}

//------------------------------------------------------------------------------

void  TCategories::setCategoriesName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );  // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );             // правленное имя кнопки
    m_ptBtnName->setToolTip( m_zName );             // подсказка с оригинальным именем
}

QString  TCategories::getCategoriesName()
{
    return m_zName;
}

int  TCategories::getCategoriesDepth()
{
    return m_depth;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

TParam::TParam( TCategories  *pMentor, int  depth )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Parameters",  this );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    m_pMentor = pMentor;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < depth; i++ )
    {
        QLabel *label = new QLabel( this, Q_NULLPTR );
        label->setMinimumWidth( 93 );
        hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-", this );
    m_ptBtnDec->setToolTip( "Удалить параметр" );
    m_ptBtnDec->setFixedWidth( 93 );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button", this );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );

    this->setLayout( m_vlayout );
}

TParam::~TParam()
{
    /*
    QLayoutItem *child;

    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    for( auto& it : m_apCategoriesList )
    {
        it->~TCategories();
    }
    */

//    clear();
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

    m_vList.clear();

    m_apCategoriesList.clear();
}

//------------------------------------------------------------------------------

void  TParam::onBtnDec()
{
    qDebug() << getParamName() << "dec button";

    ParamDelete();
}

void  TParam::onBtnInc()
{
    qDebug() << getParamName() << "inc button";
}

void  TParam::onBtnName()
{
    qDebug() << getParamName() << "button";

    m_ptDialog->setDlgName( getParamName() );
    m_ptDialog->setDlgPlaceholder( getParamPlaceholder() );
    m_ptDialog->setDlgNew( getParamNew() );
    m_ptDialog->setDlgAfter( getParamAfter() );
    m_ptDialog->setDlgBefore( getParamBefore() );
    m_ptDialog->setDlgUlink( getParamUlink() );
    m_ptDialog->setDlgUname( getParamUname() );
    m_ptDialog->setDlgMulti( getParamMulti() );

    m_ptDialog->setDlgType( getParamType() );
    m_ptDialog->setDlgMin( getParamMin() );
    m_ptDialog->setDlgMax( getParamMax() );

    m_ptDialog->setDlgCombo( getParamList() );

    m_ptDialog->open();
}

void  TParam::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    setParamName( m_tValues.m_zName.toStdString() );
}

//------------------------------------------------------------------------------

void  TParam::ParamDelete()
{
    QLayoutItem *child;

    // уничтожаем диалог
    m_ptDialog->~TDialog();

    // для всех вложенных Categories вызываем очистку
    for( auto& it : m_apCategoriesList )
    {
        it->CategoriesDelete();

        it->~TCategories();
    }

    // уничтожаем виджеты
    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем layout
    m_vlayout->deleteLater();

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pMentor )
    {
        // родитель
        for( int i = 0; i < m_pMentor->m_apParamList.count(); i++ )
        {
            if( this == m_pMentor->m_apParamList.at(i) )
            {
                qDebug() << m_pMentor->m_apParamList.at(i)->getParamName() << "obsolete";

                m_pMentor->m_apParamList.removeAt(i);
            }
        }
    }/*
    else
    {
        // прародитель
        for( int i = 0; i < TCategory::getMasterPointer()->m_apParamList.count(); i++ )
        {
            if( this == TCategory::getMasterPointer()->m_apParamList.at(i) )
            {
                qDebug() << TCategory::getMasterPointer()->m_apParamList.at(i)->getParamName() << "obsolete";

                TCategory::getMasterPointer()->m_apParamList.removeAt(i);
            }
        }
    }*/
}

//------------------------------------------------------------------------------

void  TParam::setParamName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );  // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );             // правленное имя кнопки
    m_ptBtnName->setToolTip( m_zName );             // подсказка с оригинальным именем
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


void  TParam::setParamList( QStringList  list )
{
    m_vList = list;
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


QStringList  TParam::getParamList()
{
    return m_vList;
}

//------------------------------------------------------------------------------

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
