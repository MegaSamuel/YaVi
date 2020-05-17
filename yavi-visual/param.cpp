#include "func.h"
#include "param.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategories::TCategories( TParam  *pMentor, int  depth )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Categories",  this );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    m_pMentor = pMentor;

    m_depth = depth;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth; i++ )
    {
        QLabel *label = new QLabel();
        label->setMinimumWidth( 93 );
        hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-");
    m_ptBtnDec->setToolTip( "Удалить параметр" );
    m_ptBtnDec->setFixedWidth( 93 );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    setIncBtnVisible( false );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );

    setLayout( m_vlayout );
}

TCategories::~TCategories()
{

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

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgEnabled( false );

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

void  TCategories::setIncBtnVisible( bool visible )
{
    m_ptBtnInc->setVisible( visible );
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

TParam::TParam( TCategory  *pAncestor, TCategories  *pMentor, int  depth )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Parameters",  this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialog, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    // указатель на прародителя (имеется у первого, по дереву, TParam)
    m_pAncestor = pAncestor;

    // указатель на родителя (имеется со второго, по дереву, TParam)
    m_pMentor = pMentor;

    // глубина вложения
    m_depth = depth;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth; i++ )
    {
        QLabel *label = new QLabel();
        label->setMinimumWidth( 93 );
        hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-" );
    m_ptBtnDec->setToolTip( "Удалить параметр" );
    m_ptBtnDec->setFixedWidth( 93 );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    setIncBtnVisible( false );  // по умолчанию кнопка невидимая
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );

    setLayout( m_vlayout );
}

TParam::~TParam()
{

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

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TParam::onBtnDec()
{
//    qDebug() << getParamName() << "dec button";

    ParamDelete();
}

void  TParam::onBtnValDec()
{
    qDebug() << getParamName() << "dec val button";

    setParamType( 0, true );
}

void  TParam::onBtnInc()
{
//    qDebug() << getParamName() << "inc button";

    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->open();
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

void  TParam::onSendCancel()
{
    need_to_add = false;
}

void  TParam::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues; // зачем это кладем во внутреннюю переменную??

    if( false == need_to_add )
    {
        // редактируем текущий набор параметров

        setParamName( m_tValues.m_zName.toStdString(), true );
        setParamType( m_tValues.m_uType, true );
        setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString(), true );
        setParamNew( m_tValues.m_zNew.toStdString(), true );
        setParamAfter( m_tValues.m_zAfter.toStdString(), true );
        setParamBefore( m_tValues.m_zBefore.toStdString(), true );
        setParamUlink( m_tValues.m_zUlink.toStdString(), true );
        setParamUname( m_tValues.m_zUname.toStdString(), true );
        setParamMulti( m_tValues.m_zMulti.toStdString(), true );
        setParamMin( m_tValues.m_uMin, true );
        setParamMax( m_tValues.m_uMax, true );
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров
/*
        TParam  *pParam;
        pParam = new TParam( m_pAncestor, m_pMentor, m_depth );
        pParam->setNode( m_node );

        // добавляемся к родителю
        if( Q_NULLPTR != m_pAncestor )
        {
            m_pAncestor->drawParam( pParam );
            m_pAncestor->m_apParamList.append( pParam );
        }
        else if( Q_NULLPTR != m_pMentor )
        {
            m_pMentor->m_apParamList.append( pParam );
        }

        // ставим значения параметров и пишем их в ямл
        pParam->setParamName( m_tValues.m_zName.toStdString(), true );
        pParam->setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString(), true );
        pParam->setParamNew( m_tValues.m_zNew.toStdString(), true );
        pParam->setParamAfter( m_tValues.m_zAfter.toStdString(), true );
        pParam->setParamBefore( m_tValues.m_zBefore.toStdString(), true );
        pParam->setParamUlink( m_tValues.m_zUlink.toStdString(), true );
        pParam->setParamUname( m_tValues.m_zUname.toStdString(), true );
        pParam->setParamMulti( m_tValues.m_zMulti.toStdString(), true );

        pParam->setParamType( m_tValues.m_uType, true );
        pParam->setParamMin( m_tValues.m_uMin, true );
        pParam->setParamMax( m_tValues.m_uMax, true );
*/
    }

//    setParamList( m_tValues.m_vList, true );

    need_to_add = false;
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

    qDebug() << getParamName() << "del dialog";

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

    qDebug() << getParamName() << "del categories";

    // уничтожаем виджеты
    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    qDebug() << getParamName() << "del widgets";

    // уничтожаем layout
    m_vlayout->deleteLater();

    qDebug() << getParamName() << "del layout";

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pMentor )
    {
        // родитель

        clearNodeSequence();

        for( int i = 0; i < m_pMentor->m_apParamList.count(); i++ )
        {
            if( this == m_pMentor->m_apParamList.at(i) )
            {
                qDebug() << m_pMentor->m_apParamList.at(i)->getParamName() << "obsolete (mentor)";

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
                qDebug() << m_pAncestor->m_apParamList.at(i)->getParamName() << "obsolete (ancestor)";

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

YAML::Node&  TParam::getNode()
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
        __yaml_SetString( m_node, GoodsNameSection, name );
    }
}

void  TParam::setParamPlaceholder( const std::string&  name, bool  set_to_node )
{
    m_zPlaceholder = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsPlaceholderSection, name );
    }
}

void  TParam::setParamNew( const std::string&  name, bool  set_to_node )
{
    m_zNew = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsNewSection, name );
    }
}

void  TParam::setParamAfter( const std::string&  name, bool  set_to_node )
{
    m_zAfter = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsAfterSection, name );
    }
}

void  TParam::setParamBefore( const std::string&  name, bool  set_to_node )
{
    m_zBefore = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsBeforeSection, name );
    }
}

void  TParam::setParamUlink( const std::string&  name, bool  set_to_node )
{
    m_zUlink = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsUlinkSection, name );
    }
}

void  TParam::setParamUname( const std::string&  name, bool  set_to_node )
{
    m_zUname = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsUnameSection, name );
    }
}

void  TParam::setParamMulti( const std::string&  name, bool  set_to_node )
{
    m_zMulti = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsMultiSection, name );
    }
}


void  TParam::setParamType( unsigned  val, bool  set_to_node )
{
    m_uType = val;

    qDebug() << getParamName() << "set param" << m_uType << m_second_row_exist;

    if( ( 1 == m_uType ) || ( 2 == m_uType ) || ( 3 == m_uType ) )
    {
        setParamValueAdd();
    }
    else
    {
        setParamValueDel();
    }

    if( ( 4 == m_uType ) || ( 5 == m_uType ) )
    {
        setIncBtnVisible( true );
    }
    else
    {
        setIncBtnVisible( false );
    }

    if( set_to_node )
    {
        __yaml_SetScalar( m_node, GoodsTypeSection, val );
    }
}

void  TParam::setParamMin( unsigned  val, bool  set_to_node )
{
    m_uMin = val;

    setParamValueMin( static_cast<int>(m_uMin) );

    if( set_to_node )
    {
        __yaml_SetScalar( m_node, GoodsMinSection, val );
    }
}

void  TParam::setParamMax( unsigned  val, bool  set_to_node )
{
    m_uMax = val;

    setParamValueMax( static_cast<int>(m_uMax) );

    if( set_to_node )
    {
        __yaml_SetScalar( m_node, GoodsMaxSection, val );
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

void  TParam::setIncBtnVisible( bool visible )
{
    m_ptBtnInc->setVisible( visible );
}

//------------------------------------------------------------------------------

void  TParam::setParamValueAdd()
{
    if( m_second_row_exist )
        return;

    m_hlayout = new QHBoxLayout();
    m_hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth+1; i++ )
    {
        QLabel *label = new QLabel();
        label->setMinimumWidth( 93 );
        m_hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnValDec = new QPushButton( "-" );
    m_ptBtnValDec->setToolTip( "Удалить значение" );
    m_ptBtnValDec->setFixedWidth( 93 );
    connect( m_ptBtnValDec, SIGNAL(clicked()), this, SLOT(onBtnValDec()) );
    m_hlayout->addWidget( m_ptBtnValDec, 0, Qt::AlignLeft );

    // спин со значением
    m_ptSpinValue = new QSpinBox();
    m_ptSpinValue->setToolTip( "Значение" );
    m_ptSpinValue->setFixedWidth( 93 );
    m_hlayout->addWidget( m_ptSpinValue, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout );

    m_second_row_exist = true;
}

void  TParam::setParamValueDel()
{
    if( !m_second_row_exist )
        return;

    QLayoutItem *child;

    // уничтожаем виджеты
    while( ( child = m_hlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем layout
    m_hlayout->deleteLater();

    m_second_row_exist = false;
}

void  TParam::setParamValueMin( int  min )
{
    if( m_second_row_exist )
    {
        m_ptSpinValue->setMinimum( min );
    }
}

void  TParam::setParamValueMax( int  max )
{
    if( m_second_row_exist )
    {
        m_ptSpinValue->setMaximum( max );
    }
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
