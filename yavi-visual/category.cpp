#include "func.h"
#include "goods.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategory::TCategory( TGoods  *pAncestor )
{
    m_zName.clear();

    m_depth = 0;

    // диалог
    m_ptDialog = new TDialog( true, "Category", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialog, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&) ) );

    // указатель на родителя
    m_pAncestor = pAncestor;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );

    setLayout( m_vlayout );
}

TCategory::~TCategory()
{

}

//------------------------------------------------------------------------------

void  TCategory::onBtnName()
{
//    qDebug() << getCategoryName() << "button";

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgEnabled( false );

    m_ptDialog->setDlgName( getCategoryName() );

    m_ptDialog->open();
}

void  TCategory::onBtnInc()
{
//    qDebug() << getCategoryName() << "inc button";

    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgEnabled( true );

    m_ptDialog->setDlgName( "NewParam" );

    m_ptDialog->open();
}

void  TCategory::onSendCancel()
{
    need_to_add = false;
}

void  TCategory::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    if( false == need_to_add )
    {
        // редактируем текущий набор параметров

        setCategoryName( m_tValues.m_zName.toStdString(), true );
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров

        TParam  *pParam;
        pParam = new TParam( this, Q_NULLPTR, m_depth );
        pParam->setNode( m_node[ GoodsParametersSection ] );

        // добавляемся к родителю
        // в ямле новые параметры добавляются в конец, делаем так же
        //m_vlayout->insertWidget( 1, pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_apParamList.append( pParam );

        // ставим значения параметров
        pParam->setParamName( m_tValues.m_zName.toStdString(), false );
        pParam->setParamType( m_tValues.m_uType, false );
        pParam->setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString(), false );
        pParam->setParamNew( m_tValues.m_zNew.toStdString(), false );
        pParam->setParamAfter( m_tValues.m_zAfter.toStdString(), false );
        pParam->setParamBefore( m_tValues.m_zBefore.toStdString(), false );
        pParam->setParamUlink( m_tValues.m_zUlink.toStdString(), false );
        pParam->setParamUname( m_tValues.m_zUname.toStdString(), false );
        pParam->setParamMulti( m_tValues.m_zMulti.toStdString(), false );
        pParam->setParamMin( m_tValues.m_uMin, false );
        pParam->setParamMax( m_tValues.m_uMax, false );

        YAML::Node  node;
        node.reset();

        // пишем их в ямл
        __yaml_SetString( node, GoodsNameSection, m_tValues.m_zName.toStdString() );
        __yaml_SetScalar( node, GoodsTypeSection, m_tValues.m_uType );
        __yaml_SetString( node, GoodsPlaceholderSection, m_tValues.m_zPlaceholder.toStdString() );
        __yaml_SetString( node, GoodsNewSection, m_tValues.m_zNew.toStdString() );
        __yaml_SetString( node, GoodsAfterSection, m_tValues.m_zAfter.toStdString() );
        __yaml_SetString( node, GoodsBeforeSection, m_tValues.m_zBefore.toStdString() );
        __yaml_SetString( node, GoodsUlinkSection, m_tValues.m_zUlink.toStdString() );
        __yaml_SetString( node, GoodsUnameSection, m_tValues.m_zUname.toStdString() );
        __yaml_SetString( node, GoodsMultiSection, m_tValues.m_zMulti.toStdString() );
        __yaml_SetScalar( node, GoodsMinSection, m_tValues.m_uMin );
        __yaml_SetScalar( node, GoodsMaxSection, m_tValues.m_uMax );

        m_node[ GoodsParametersSection ].push_back( node );
    }

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TCategory::CategoryDelete()
{
    QLayoutItem *child;

    // уничтожаем диалог
    m_ptDialog->~TDialog();

/* тут падает
    // для всех вложенных Parameters вызываем очистку
    for( auto& it : m_apParamList )
    {
        // очищаем
        it->ParamDelete();

        // уничтожаем
        it->~TParam();
    }
*/
//    qDebug() << getCategoryName() << "del param";

    // уничтожаем виджеты
    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем layout
    m_vlayout->deleteLater();

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pAncestor )
    {
        for( int i = 0; i < m_pAncestor->m_apCategoryList.count(); i++ )
        {
            if( this == m_pAncestor->m_apCategoryList.at(i) )
            {
//                qDebug() << m_pAncestor->m_apCategoryList.at(i)->getCategoryName() << "obsolete";

                m_pAncestor->m_apCategoryList.removeAt(i);
            }
        }
    }
}

void  TCategory::setNode( const YAML::Node&  node )
{
    m_node = node;
}

YAML::Node&  TCategory::getNode()
{
    return m_node;
}

void  TCategory::getCategories( const YAML::Node&  node, TParam  *a_pParam, int  depth )
{
    std::string  str;

    TCategories  *pCategories;
    pCategories = new TCategories( a_pParam, depth );
    pCategories->setNode( node );
    m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );

    // добавляем Categories с список класса TParam
    a_pParam->m_apCategoriesList.append(pCategories);

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    pCategories->setCategoriesName( str );

    //
    str = __yaml_GetString( node, GoodsUlinkSection );
    pCategories->setCategoriesUlink( str );

    //
    str = __yaml_GetString( node, GoodsUnameSection );
    pCategories->setCategoriesUname( str );

    if( __yaml_IsSequence( node[ GoodsParametersSection ] ) )
    {
        for( auto& par : node[ GoodsParametersSection ] )
        {
            TParam  *pParam;
            pParam = new TParam( Q_NULLPTR, pCategories, pCategories->getCategoriesDepth()+1 );
            pParam->setNode( par );

            // добавляем Parameters в список класса TCategories
            pCategories->m_apParamList.append(pParam);

            getParameters( par, pParam, pCategories->getCategoriesDepth()+1 );
        }
    }
}

void  TCategory::getParameters( const YAML::Node&  node, TParam *a_pParam, int  depth )
{
    unsigned  val;
    std::string  str;

    m_vlayout->addWidget( a_pParam, 0, Qt::AlignLeft | Qt::AlignTop );
    m_apParamList.append( a_pParam );

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    a_pParam->setParamName( str );

    // тип
    if( __yaml_IsScalar( node[ GoodsTypeSection ] ) )
    {
        val = node[ GoodsTypeSection ].as<unsigned>();
        a_pParam->setParamType( val );
    }

    //
    str = __yaml_GetString( node, GoodsPlaceholderSection );
    a_pParam->setParamPlaceholder( str );

    //
    str = __yaml_GetString( node, GoodsNewSection );
    a_pParam->setParamNew( str );

    //
    str = __yaml_GetString( node, GoodsAfterSection );
    a_pParam->setParamAfter( str );

    //
    str = __yaml_GetString( node, GoodsBeforeSection );
    a_pParam->setParamBefore( str );

    //
    str = __yaml_GetString( node, GoodsUlinkSection );
    a_pParam->setParamUlink( str );

    //
    str = __yaml_GetString( node, GoodsUnameSection );
    a_pParam->setParamUname( str );

    // мин
    if( __yaml_IsScalar( node[ GoodsMinSection ] ) )
    {
        val = node[ GoodsMinSection ].as<unsigned>();
        a_pParam->setParamMin( val );
    }

    // макс
    if( __yaml_IsScalar( node[ GoodsMaxSection ] ) )
    {
        val = node[ GoodsMaxSection ].as<unsigned>();
        a_pParam->setParamMax( val );
    }

    //
    str = __yaml_GetString( node, GoodsMultiSection );
    a_pParam->setParamMulti( str );

    // значение
    str = __yaml_GetString( node, GoodsValuesSection );
    QStringList list = QString::fromStdString(str).split( '\n', QString::SkipEmptyParts );
    a_pParam->setParamList( list, str );

    if( 0 != QString::fromStdString(str).length() )
    {
        if( __yaml_IsSequence( node[ GoodsCategoriesSection ] ) )
        {
            for( auto& cat : node[ GoodsCategoriesSection ] )
            {
                getCategories( cat, a_pParam, depth+1 );
            }
        }
    }
}

//------------------------------------------------------------------------------

/*
void  TCategory::drawParam( TParam  *pParam )
{
    if( Q_NULLPTR != pParam )
    {
        m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
    }
}
*/

void  TCategory::setCategoryName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );  // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );             // правленное имя кнопки
    m_ptBtnName->setToolTip( m_zName );             // подсказка с оригинальным именем

    if( set_to_node )
    {
        bool res = __yaml_SetString( m_node, GoodsCategoryName, name );

        qDebug() << "set" << GoodsCategoryName << m_zName << "result" << res;
    }
}

const QString TCategory::getCategoryName()
{
    return m_zName;
}

//------------------------------------------------------------------------------

int TCategory::getCategoryWidth()
{
    QSize size = m_vlayout->minimumSize();

    return size.width();
}

int TCategory::getCategoryHeight()
{
    QSize size = m_vlayout->minimumSize();

    return size.height();
}

//------------------------------------------------------------------------------
