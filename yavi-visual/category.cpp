#include "func.h"
#include "goods.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategory::TCategory( TGoods  *pAncestor )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Category", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialog, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&) ) );

    // указатель на родителя
    m_pAncestor = pAncestor;

    m_depth = 0;

    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin(0);


    m_hlayout = new QHBoxLayout;
    m_hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    m_hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout, 0 );

    setLayout( m_vlayout );

    // ставим начальный размер себя
    widget_size_reset();

    widget_stretch( m_vlayout->minimumSize().width(), m_vlayout->minimumSize().height() );
}

TCategory::~TCategory()
{

}

void TCategory::clear()
{
    m_zName.clear();
    m_zBtnName.clear();

    m_apParamList.clear();

    need_to_add = false;
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
        //pParam->setNode( m_node[ GoodsParametersSection ] );

        // добавляемся к родителю
        // в ямле новые параметры добавляются в конец, делаем так же
        //m_vlayout->insertWidget( 1, pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_apParamList.append( pParam );

        // ставим значения параметров
        pParam->setParamName( m_tValues.m_zName.toStdString() );
        pParam->setParamType( m_tValues.m_uType );
        pParam->setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString() );
        pParam->setParamNew( m_tValues.m_zNew.toStdString() );
        pParam->setParamAfter( m_tValues.m_zAfter.toStdString() );
        pParam->setParamBefore( m_tValues.m_zBefore.toStdString() );
        pParam->setParamUlink( m_tValues.m_zUlink.toStdString() );
        pParam->setParamUname( m_tValues.m_zUname.toStdString() );
        pParam->setParamMulti( m_tValues.m_zMulti.toStdString() );
        pParam->setParamMin( m_tValues.m_uMin );
        pParam->setParamMax( m_tValues.m_uMax );

        widget_stretch( pParam->getParamWidth(), pParam->getParamHeight() );
        widget_parent_stretch( pParam->getParamWidth(), pParam->getParamHeight() );

        YAML::Node  node;
        node.reset();

        // пишем их в пустой ямл
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

        // добавляем ямл к основному
        m_node[ GoodsParametersSection ].push_back( node );

        int index = static_cast<int>(m_node[ GoodsParametersSection ].size()) - 1;

        pParam->setNode( m_node[ GoodsParametersSection ][index] );
        pParam->setNodeParent( m_node[ GoodsParametersSection ] );
        pParam->setNodeIndex( index );

        qDebug() << pParam->getParamName() << "index" << index;
    }

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TCategory::CategoryDelete()
{
    QLayoutItem *child;

    // уничтожаем диалог
    m_ptDialog->~TDialog();

    // для всех вложенных Parameters вызываем очистку
    for( auto& it : m_apParamList )
    {
        // очищаем
        it->ParamDelete();

        // уничтожаем
        it->~TParam();
    }

    // уничтожаем виджеты
    while( ( child = m_hlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
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
    if( Q_NULLPTR != m_pAncestor )
    {
        // удаляемся из списка детей
        for( int i = 0; i < m_pAncestor->m_apCategoryList.count(); i++ )
        {
            if( this == m_pAncestor->m_apCategoryList.at(i) )
            {
//                qDebug() << m_pAncestor->m_apCategoryList.at(i)->getCategoryName() << "obsolete";

                m_pAncestor->m_apCategoryList.removeAt(i);

                break;
            }
        }

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_pAncestor->m_apCategoryList.count(); i++ )
        {
            index = m_pAncestor->m_apCategoryList.at(i)->getNodeIndex();

            if( index > m_node_index )
            {
                index -= 1;

                m_pAncestor->m_apCategoryList.at(i)->setNodeIndex( index );
            }
        }
    }
}

//------------------------------------------------------------------------------

void  TCategory::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TCategory::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TCategory::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TCategory::getNode()
{
    return m_node;
}

YAML::Node&  TCategory::getNodeParent()
{
    return m_node_parent;
}

int  TCategory::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TCategory::getCategories( const YAML::Node&  node, TCategories  *a_pCategories, int  depth )
{
    std::string  str;

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    a_pCategories->setCategoriesName( str );

    //
    str = __yaml_GetString( node, GoodsUlinkSection );
    a_pCategories->setCategoriesUlink( str );

    //
    str = __yaml_GetString( node, GoodsUnameSection );
    a_pCategories->setCategoriesUname( str );

    if( __yaml_IsSequence( node[ GoodsParametersSection ] ) )
    {
        TParam  *pParam;

        for( int i = 0; i < static_cast<int>(node[ GoodsParametersSection ].size()); i++ )
        {
            pParam = new TParam( Q_NULLPTR, a_pCategories, depth+1 );
            pParam->setNode( node[ GoodsParametersSection ][i] );
            pParam->setNodeParent( node[ GoodsParametersSection ] );
            pParam->setNodeIndex( i );

            a_pCategories->m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
            a_pCategories->m_apParamList.append( pParam );

            getParameters( node[ GoodsParametersSection ][i], pParam, pParam->getParamDepth() );

            // подгоняем размер виджета под содержимое для корректной работы скролла
            widget_stretch( pParam->getParamWidth(), pParam->getParamHeight() );
            widget_parent_stretch( pParam->getParamWidth(), pParam->getParamHeight() );
        }
    }

    // подгоняем размер виджета под содержимое для корректной работы скролла
    //widget_stretch( a_pCategories->getCategoriesWidth(), a_pCategories->getCategoriesHeight() );
}

void  TCategory::getParameters( const YAML::Node&  node, TParam *a_pParam, int  depth )
{
    unsigned  val;
    std::string  str;

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
            TCategories  *pCategories;

            for( int i = 0; i < static_cast<int>(node[ GoodsCategoriesSection ].size()); i++ )
            {
                pCategories = new TCategories( a_pParam, depth+1 );
                pCategories->setNode( node[ GoodsCategoriesSection ][i] );
                pCategories->setNodeParent( node[ GoodsCategoriesSection ] );
                pCategories->setNodeIndex( i );

                a_pParam->m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );
                a_pParam->m_apCategoriesList.append( pCategories );

                getCategories( node[ GoodsCategoriesSection ][i], pCategories, pCategories->getCategoriesDepth() );

                // подгоняем размер виджета под содержимое для корректной работы скролла
                widget_stretch( pCategories->getCategoriesWidth(), pCategories->getCategoriesHeight() );
                widget_parent_stretch( pCategories->getCategoriesWidth(), pCategories->getCategoriesHeight() );
            }

            // подгоняем размер виджета под содержимое для корректной работы скролла
            //widget_stretch( getCategoryWidth(), getCategoryHeight() );
        }
    }
}

//------------------------------------------------------------------------------

void  TCategory::setCategoryName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Категория: " + m_zName );  // подсказка с оригинальным именем

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsCategoryName, name );
    }
}

const QString TCategory::getCategoryName()
{
    return m_zName;
}

//------------------------------------------------------------------------------

void  TCategory::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;

    m_width = 2 * m_vlayout->margin();
    m_height = 2 * m_vlayout->margin();

    //qDebug() << "init category size" << m_width << m_height;
}

void  TCategory::widget_stretch( int width, int height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height = height;

    //qDebug() << "cat stretch" << width << height << m_width << m_height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );
}

void  TCategory::widget_parent_stretch( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_stretch( width, height );
    }
}

void  TCategory::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumHeight( m_height );

    widget_parent_shrink( m_width, m_height );
}

void  TCategory::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_shrink( width, height );
    }
}

int TCategory::getCategoryWidth()
{
    return m_vlayout->minimumSize().width();
}

int TCategory::getCategoryHeight()
{
    return m_vlayout->minimumSize().height();
}

//------------------------------------------------------------------------------
