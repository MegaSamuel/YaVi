#include "func.h"
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
//    qDebug() << getCategoriesName() << "dec button";

    CategoriesDelete();
}

void  TCategories::onBtnInc()
{
//    qDebug() << getCategoriesName() << "inc button";
}

void  TCategories::onBtnName()
{
//    qDebug() << getCategoriesName() << "button";

    m_ptDialog->setDlgName( getCategoriesName() );

    m_ptDialog->open();
}

void  TCategories::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    setCategoriesName( m_tValues.m_zName.toStdString(), true );
}

//------------------------------------------------------------------------------


void  TCategories::clearNodeSequence()
{
    m_node.remove( GoodsNameSection );
    m_node.remove( GoodsTypeSection );
    m_node.remove( GoodsPlaceholderSection );
    m_node.remove( GoodsNewSection );
    m_node.remove( GoodsAfterSection );
    m_node.remove( GoodsBeforeSection );
    m_node.remove( GoodsUlinkSection );
    m_node.remove( GoodsUnameSection );
    m_node.remove( GoodsMinSection );
    m_node.remove( GoodsMaxSection );
    m_node.remove( GoodsMultiSection );
    m_node.remove( GoodsValuesSection );

    m_node.remove( GoodsParametersSection );
}

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
        // удаляем ветку из ямла
//        bool res = __yaml_DelNode( m_node, it->getNode() );

//        qDebug() << getCategoriesName() << "del node" << res;

//        m_node.remove( GoodsParametersSection );
//        bool res = m_node.remove( it->getNode() );
//        qDebug() << getCategoriesName() << it->getParamName()  << "del node" << res;

        // очищаем
        it->ParamDelete();

        // уничтожаем
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
        clearNodeSequence();

        for( int i = 0; i < m_pMentor->m_apCategoriesList.count(); i++ )
        {
            if( this == m_pMentor->m_apCategoriesList.at(i) )
            {
//                qDebug() << m_pMentor->m_apCategoriesList.at(i)->getCategoriesName() << "obsolete";

                m_pMentor->m_apCategoriesList.removeAt(i);
            }
        }
    }
}

//------------------------------------------------------------------------------

void  TCategories::setNode( const YAML::Node&  node )
{
    m_node = node;
}

YAML::Node&  TCategories::getNode()
{
    return m_node;
}

void  TCategories::setCategoriesName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );  // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );             // правленное имя кнопки
    m_ptBtnName->setToolTip( m_zName );             // подсказка с оригинальным именем

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsNameSection, name );

        qDebug() << "set" << GoodsNameSection << m_zName << res;
    }
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

TParam::TParam( TCategory  *pAncestor, TCategories  *pMentor, int  depth )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Parameters",  this );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    // указатель на прародителя (имеется у первого, по дереву, TParam)
    m_pAncestor = pAncestor;

    // указатель на родителя (имеется со второго, по дереву, TParam)
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
//    qDebug() << getParamName() << "dec button";

    ParamDelete();
}

void  TParam::onBtnInc()
{
//    qDebug() << getParamName() << "inc button";
}

void  TParam::onBtnName()
{
//    qDebug() << getParamName() << "button";

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

    setParamName( m_tValues.m_zName.toStdString(), true );
    setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString(), true );
    setParamNew( m_tValues.m_zNew.toStdString(), true );
    setParamAfter( m_tValues.m_zAfter.toStdString(), true );
    setParamBefore( m_tValues.m_zBefore.toStdString(), true );
    setParamUlink( m_tValues.m_zUlink.toStdString(), true );
    setParamUname( m_tValues.m_zUname.toStdString(), true );
    setParamMulti( m_tValues.m_zMulti.toStdString(), true );

    setParamType( m_tValues.m_uType, true );
    setParamMin( m_tValues.m_uMin, true );
    setParamMax( m_tValues.m_uMax, true );

//    setParamList( m_tValues.m_vList, true );
}

//------------------------------------------------------------------------------

void  TParam::clearNodeSequence()
{
    m_node.remove( GoodsNameSection );
    m_node.remove( GoodsTypeSection );
    m_node.remove( GoodsPlaceholderSection );
    m_node.remove( GoodsNewSection );
    m_node.remove( GoodsAfterSection );
    m_node.remove( GoodsBeforeSection );
    m_node.remove( GoodsUlinkSection );
    m_node.remove( GoodsUnameSection );
    m_node.remove( GoodsMinSection );
    m_node.remove( GoodsMaxSection );
    m_node.remove( GoodsMultiSection );
    m_node.remove( GoodsValuesSection );

    m_node.remove( GoodsCategoriesSection );
}

void  TParam::ParamDelete()
{
    QLayoutItem *child;

    // уничтожаем диалог
    m_ptDialog->~TDialog();

    // для всех вложенных Categories вызываем очистку
    for( auto& it : m_apCategoriesList )
    {
        // удаляем ветку из ямла
//        bool res = __yaml_DelNode( m_node, it->getNode() );

//        qDebug() << getParamName() << "del node" << res;

        // очищаем
        it->CategoriesDelete();

        // уничтожаем
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

        clearNodeSequence();

        for( int i = 0; i < m_pMentor->m_apParamList.count(); i++ )
        {
            if( this == m_pMentor->m_apParamList.at(i) )
            {
//                qDebug() << m_pMentor->m_apParamList.at(i)->getParamName() << "obsolete (mentor)";

                m_pMentor->m_apParamList.removeAt(i);
            }
        }
    }
    else if( Q_NULLPTR != m_pAncestor )
    {
        // прародитель

        // удаляем ветку из ямла
//        bool res = __yaml_DelNode( m_pAncestor->getNode(), m_node );

//        bool res = m_pAncestor->getNode().remove( 1 );

//        bool res = m_node.remove( "type" );

//        qDebug() << getParamName() << "del node (ancestor)" << res;

        clearNodeSequence();

//        y.remove(y["b"]);

//        m_pAncestor->getNode().remove( m_node );

        for( int i = 0; i < m_pAncestor->m_apParamList.count(); i++ )
        {
            if( this == m_pAncestor->m_apParamList.at(i) )
            {
//                qDebug() << m_pAncestor->m_apParamList.at(i)->getParamName() << "obsolete (ancestor)";

                m_pAncestor->m_apParamList.removeAt(i);
            }
        }
    }
}

//------------------------------------------------------------------------------

void  TParam::setNode( const YAML::Node&  node )
{
    m_node = node;
}

YAML::Node  TParam::getNode()
{
    return m_node;
}

void  TParam::setParamName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );  // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );             // правленное имя кнопки
    m_ptBtnName->setToolTip( m_zName );             // подсказка с оригинальным именем

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsNameSection, name );

        qDebug() << "set" << GoodsNameSection << m_zName << res;
    }
}

void  TParam::setParamPlaceholder( const std::string&  name, bool  set_to_node )
{
    m_zPlaceholder = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsPlaceholderSection, name );

        qDebug() << "set" << GoodsPlaceholderSection << m_zPlaceholder << res;
    }
}

void  TParam::setParamNew( const std::string&  name, bool  set_to_node )
{
    m_zNew = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsNewSection, name );

        qDebug() << "set" << GoodsNewSection << m_zNew << res;
    }
}

void  TParam::setParamAfter( const std::string&  name, bool  set_to_node )
{
    m_zAfter = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsAfterSection, name );

        qDebug() << "set" << GoodsAfterSection << m_zAfter << res;
    }
}

void  TParam::setParamBefore( const std::string&  name, bool  set_to_node )
{
    m_zBefore = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsBeforeSection, name );

        qDebug() << "set" << GoodsBeforeSection << m_zBefore << res;
    }
}

void  TParam::setParamUlink( const std::string&  name, bool  set_to_node )
{
    m_zUlink = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsUlinkSection, name );

        qDebug() << "set" << GoodsUlinkSection << m_zUlink << res;
    }
}

void  TParam::setParamUname( const std::string&  name, bool  set_to_node )
{
    m_zUname = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsUnameSection, name );

        qDebug() << "set" << GoodsUnameSection << m_zUname << res;
    }
}

void  TParam::setParamMulti( const std::string&  name, bool  set_to_node )
{
    m_zMulti = QString::fromStdString(name);

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsMultiSection, name );

        qDebug() << "set" << GoodsMultiSection << m_zMulti << res;
    }
}


void  TParam::setParamType( unsigned  val, bool  set_to_node )
{
    m_uType = val;

    if( set_to_node )
    {
        bool res = __yaml_SetScalar( m_node, GoodsTypeSection, val );

        qDebug() << "set" << GoodsTypeSection << m_uType << res;
    }
}

void  TParam::setParamMin( unsigned  val, bool  set_to_node )
{
    m_uMin = val;

    if( set_to_node )
    {
        bool res = __yaml_SetScalar( m_node, GoodsMinSection, val );

        qDebug() << "set" << GoodsMinSection << m_uMin << res;
    }
}

void  TParam::setParamMax( unsigned  val, bool  set_to_node )
{
    m_uMax = val;

    if( set_to_node )
    {
        bool res = __yaml_SetScalar( m_node, GoodsMaxSection, val );

        qDebug() << "set" << GoodsMaxSection << m_uMax << res;
    }
}


void  TParam::setParamList( QStringList  list, bool  set_to_node )
{
    m_vList = list;

    if( set_to_node )
    {

    }
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
