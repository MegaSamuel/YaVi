#include "func.h"
#include "goods.h"
#include "category.h"
#include "mainwindow.h"

//------------------------------------------------------------------------------

TCategory::TCategory( TGoods  *pAncestor )
{
    clear();

    // диалог для себя
    m_ptDialogSelf = new TDialog( false, "Category", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialogSelf, &TDialog::sendCancel, this, &TCategory::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptDialogSelf, &TDialog::sendValues, this, &TCategory::onSendValues );

    // диалог для добавления параметра
    m_ptDialogAdd = new TDialog( true, "Add parameter", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialogAdd, &TDialog::sendCancel, this, &TCategory::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptDialogAdd, &TDialog::sendValues, this, &TCategory::onSendValues );

    // указатель на родителя
    m_pAncestor = pAncestor;

    m_depth = 0;

    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    // внутри вызова YAML::Node() есть new, чтобы избежать утечек памяти заводим переменную в классе
    m_temporary_node = YAML::Node();

    // цепляем местный сигнал к слоту MainWindow
    connect( this, &TCategory::sendChanged, MainWindow::getMainWinPtr(), &MainWindow::onYamlChanged );

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin(0);

    m_hlayout = new QHBoxLayout;
    m_hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_hlayout->setMargin(0);

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, &QPushButton::clicked, this, &TCategory::onBtnName );
    m_hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, &QPushButton::clicked, this, &TCategory::onBtnInc );
    m_hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout, 0 );

    setLayout( m_vlayout );

    widget_size_reset();

    int  width = 2*m_vlayout->margin() + 2*m_ptBtnInc->minimumSize().width() + 1*m_vlayout->spacing();
    int  height = 2*m_vlayout->margin() + m_ptBtnInc->minimumSizeHint().height();

    widget_stretch( width, height );
}

TCategory::~TCategory()
{

}

void TCategory::clear()
{
    m_zName.clear();
    m_zBtnName.clear();

    m_zUlink.clear();
    m_zUname.clear();

    m_apParamList.clear();

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TCategory::onBtnName()
{
    // диалог с пустыми параметрами
    m_ptDialogSelf->setDlgEmpty();

    m_ptDialogSelf->setDlgName( getCategoryName() );
    m_ptDialogSelf->setDlgUlink( getCategoryUlink() );
    m_ptDialogSelf->setDlgUname( getCategoryUname() );

    m_ptDialogSelf->open();
}

void  TCategory::onBtnInc()
{
    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialogAdd->setDlgEmpty();

    m_ptDialogAdd->setDlgName( "NewParam" );

    m_ptDialogAdd->open();
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
        setCategoryUlink( m_tValues.m_zUlink.toStdString(), true );
        setCategoryUname( m_tValues.m_zUname.toStdString(), true );
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров

        TParam  *pParam;
        pParam = new TParam( this, Q_NULLPTR, m_depth );

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
        pParam->setParamMin( m_tValues.m_nMin );
        pParam->setParamMax( m_tValues.m_nMax );
        pParam->setParamDMin( m_tValues.m_fMin );
        pParam->setParamDMax( m_tValues.m_fMax );

        // очищаем ямл
        m_temporary_node.reset();

        // пишем их в пустой ямл
        __yaml_SetString( m_temporary_node, GoodsNameSection, m_tValues.m_zName.toStdString() );
        __yaml_SetScalar( m_temporary_node, GoodsTypeSection, m_tValues.m_uType );

        if( 0 != m_tValues.m_uType )
        {
            __yaml_SetString( m_temporary_node, GoodsPlaceholderSection, m_tValues.m_zPlaceholder.toStdString() );
            __yaml_SetString( m_temporary_node, GoodsNewSection, m_tValues.m_zNew.toStdString() );
            __yaml_SetString( m_temporary_node, GoodsAfterSection, m_tValues.m_zAfter.toStdString() );
            __yaml_SetString( m_temporary_node, GoodsBeforeSection, m_tValues.m_zBefore.toStdString() );
            __yaml_SetString( m_temporary_node, GoodsUlinkSection, m_tValues.m_zUlink.toStdString() );
            __yaml_SetString( m_temporary_node, GoodsUnameSection, m_tValues.m_zUname.toStdString() );
        }

        if( 2 == m_tValues.m_uType )
        {
            __yaml_SetInteger( m_temporary_node, GoodsMinSection, m_tValues.m_nMin );
            __yaml_SetInteger( m_temporary_node, GoodsMaxSection, m_tValues.m_nMax );
        }
        else if( 3 == m_tValues.m_uType )
        {
            __yaml_SetDouble( m_temporary_node, GoodsMinSection, m_tValues.m_fMin );
            __yaml_SetDouble( m_temporary_node, GoodsMaxSection, m_tValues.m_fMax );
        }

        if( 5 == m_tValues.m_uType )
        {
            __yaml_SetString( m_temporary_node, GoodsMultiSection, m_tValues.m_zMulti.toStdString() );
        }

        // добавляем ямл к основному
        m_node[ GoodsParametersSection ].push_back( m_temporary_node );

        int index = static_cast<int>(m_node[ GoodsParametersSection ].size()) - 1;

        pParam->setNode( m_node[ GoodsParametersSection ][index] );
        pParam->setNodeParent( m_node[ GoodsParametersSection ] );
        pParam->setNodeIndex( index );

        pParam->setParamNameColor( pParam->getParamName() );
    }

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TCategory::CategoryDelete()
{
    QLayoutItem *child;

    // для всех вложенных Parameters вызываем очистку
    for( auto& it : m_apParamList )
    {
        // очищаем
        it->ParamDelete();

        // удаляем
        delete it;
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

        // удаляемся из родительского layout-а
        m_pAncestor->m_vlayout->removeWidget(this);
    }
}

//------------------------------------------------------------------------------

bool  TCategory::isParamNameRedefined( const QString&  name )
{
    Q_UNUSED(name);

    //qDebug() << "what to do with" << name << "?";

    return false;
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

void  TCategory::addCategories( YAML::Node&  node, TParam  *a_pParam, const std::string&  name, int  depth, int  index )
{
    TCategories  *pCategories;

    // добавляем категорию
    pCategories = new TCategories( a_pParam, depth );

    // добавляемся к родителю
    a_pParam->m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );
    a_pParam->m_apCategoriesList.append( pCategories );

    // ставим значение имени
    pCategories->setCategoriesName( name );

    // очищаем ямл
    m_temporary_node.reset();

    // пишем в пустой ямл
    __yaml_SetString( m_temporary_node, GoodsNameSection, name );

    // добавляем ямл к основному
    node[ GoodsCategoriesSection ].push_back( m_temporary_node );

    pCategories->setNode( node[ GoodsCategoriesSection ][index] );
    pCategories->setNodeParent( node[ GoodsCategoriesSection ] );
    pCategories->setNodeIndex( index );
}

void  TCategory::formCategories( const YAML::Node&  node, TParam  *a_pParam, int  depth )
{
    TCategories  *pCategories;

    // создаем categories в интерфейсе
    for( int i = 0; i < static_cast<int>(node.size()); i++ )
    {
        pCategories = new TCategories( a_pParam, depth );
        pCategories->setNode( node[i] );
        pCategories->setNodeParent( node );
        pCategories->setNodeIndex( i );

        a_pParam->m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );
        a_pParam->m_apCategoriesList.append( pCategories );

        YAML::Node  node_cat = node[i];

        getCategories( node_cat, pCategories, pCategories->getCategoriesDepth() );
    }
}

void  TCategory::getCategories( YAML::Node&  node, TCategories  *a_pCategories, int  depth )
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

            YAML::Node  node_par = node[ GoodsParametersSection ][i];

            getParameters( node_par, pParam, pParam->getParamDepth() );
        }
    }
}

void  TCategory::getParameters( YAML::Node&  node, TParam *a_pParam, int  depth )
{
    unsigned     type;
    std::string  str;
    QString      name;

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    a_pParam->setParamName( str );
    name = QString::fromStdString(str);

    // тип
    type = __yaml_GetUnsigned( node, GoodsTypeSection );
    a_pParam->setParamType( type );

    // красим после того как прочитали name и type!
    a_pParam->setParamNameColor( a_pParam->getParamName() );

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

    // мин, макс
    if( 2 == type )
    {
        a_pParam->setParamMin( __yaml_GetInteger( node, GoodsMinSection ) );
        a_pParam->setParamMax( __yaml_GetInteger( node, GoodsMaxSection ) );
    }
    else if( 3 == type )
    {
        a_pParam->setParamDMin( __yaml_GetDouble( node, GoodsMinSection ) );
        a_pParam->setParamDMax( __yaml_GetDouble( node, GoodsMaxSection ) );
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
        if( ( 4 == type ) || ( 5 == type ) )
        {
            // тип 4 или 5

            if( __yaml_IsSequence( node[ GoodsCategoriesSection ] ) )
            {
                // есть записи в values, и есть секция categories
                // приоритет у записей в values

                formCategories( node[ GoodsCategoriesSection ], a_pParam, depth+1 );

                int  list_size = list.size();
                int  cat_size = static_cast<int>(node[ GoodsCategoriesSection ].size());

                // сравниваем количество записей в values и количество элементов в списке
                if( list_size > cat_size )
                {
                    //qDebug() << "there is value bigger than categories for" << a_pParam->getParamName();

                    // дописываем недостающие categories
                    for( int i = cat_size; i < list_size; i++ )
                    {
                        addCategories( node, a_pParam, list[i].toStdString(), depth+1, i );
                    }

                    Q_EMIT  sendChanged();
                }
                else if( list_size < cat_size )
                {
                    //qDebug() << "there is value smaller than categories for" << a_pParam->getParamName();

                    // дописываем недостающие поля в values
                    for( int i = list_size; i < cat_size; i++ )
                    {
                        QString  cat_name = a_pParam->m_apCategoriesList[i]->getCategoriesName();

                        a_pParam->addParamList( cat_name, true );
                    }

                    Q_EMIT  sendChanged();
                }
            }
            else
            {
                // есть записи в values, но нет секции categories
                // создаем секцию categories и пишем в нее значения из values

                //qDebug() << "there is no categories for" << a_pParam->getParamName();

                for( auto& it : list )
                {
                    addCategories( node, a_pParam, it.toStdString(), depth+1, list.indexOf(it) );
                }

                Q_EMIT  sendChanged();
            }
        }
        else
        {
            // тип не 4 и не 5

            if( __yaml_IsSequence( node[ GoodsCategoriesSection ] ) )
            {
                // есть записи в values, и есть секция categories

                //qDebug() << name << "type != 4|5, but values and categories exist!";
            }
            else
            {
                // есть записи в values, секции categories нет

                //qDebug() << name << "type != 4|5, but values exists, categories is empty!";
            }
        }
    }
    else
    {
        if( __yaml_IsSequence( node[ GoodsCategoriesSection ] ) )
        {
            // нет записей в values, но есть секция categories

            if( ( 4 == type ) || ( 5 == type ) )
            {
                //qDebug() << name << "values is empty, but categories exists!";

                formCategories( node[ GoodsCategoriesSection ], a_pParam, depth+1 );

                // формируем values у родителя
                for( auto& it : a_pParam->m_apCategoriesList )
                {
                    QString  cat_name = it->getCategoriesName();

                    a_pParam->addParamList( cat_name, true );
                }
            }
            else
            {
                //qDebug() << name << "type != 4|5, values is empty, but categories exists!";
            }
        }
    }
}

//------------------------------------------------------------------------------

void  TCategory::setCategoryName( const std::string&  name, bool  set_to_node )
{
    int  height;

    m_zName = QString::fromStdString(name);

    height = m_ptBtnName->minimumSizeHint().height();

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Категория: " + m_zName );  // подсказка с оригинальным именем

    height = m_ptBtnName->minimumSizeHint().height() - height;

    if( 0 < height )
    {
        widget_stretch( 0, height, false );
    }
    else
    {
        widget_shrink( 0, -1 * height );
    }

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsCategoryName, name );
    }
}

void  TCategory::setCategoryUlink( const std::string&  name, bool  set_to_node )
{
    m_zUlink = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsUlinkSection, name );
    }
}

void  TCategory::setCategoryUname( const std::string&  name, bool  set_to_node )
{
    m_zUname = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsUnameSection, name );
    }
}

QString  TCategory::getCategoryName() noexcept
{
    return m_zName;
}

QString  TCategory::getCategoryUlink() noexcept
{
    return m_zUlink;
}

QString  TCategory::getCategoryUname() noexcept
{
    return m_zUname;
}

//------------------------------------------------------------------------------

void  TCategory::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;
}

void  TCategory::widget_stretch( int width, int height, bool add_height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_stretch( width, height, add_height );
}

void  TCategory::widget_parent_stretch( int width, int height, bool add_height ) noexcept
{
    int  val = 0;

    if( Q_NULLPTR != m_pAncestor )
    {
        if( add_height )
        {
            val = m_pAncestor->m_vlayout->spacing();
        }

        m_pAncestor->widget_stretch( width, height + val );
    }
}

void  TCategory::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_shrink( width, height );
}

void  TCategory::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_shrink( width, height );
    }
}

int TCategory::getCategoryWidth() noexcept
{
    return minimumSize().width();
}

int TCategory::getCategoryHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------
