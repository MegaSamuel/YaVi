#include "func.h"
#include "param.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategories::TCategories( TParam  *pMentor, int  depth )
{
    clear();

    // диалог
    m_ptDialog = new TDialog( true, "Categories",  this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialog, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialog, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    m_pMentor = pMentor;

    m_depth = depth;

    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    m_hlayout = new QHBoxLayout;
    m_hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth; i++ )
    {
        QLabel *label = new QLabel();
        label->setFixedWidth( 93 );
        m_hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-" );
    m_ptBtnDec->setToolTip( "Удалить параметр" );
    m_ptBtnDec->setFixedWidth( 93 );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    m_hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    m_hlayout->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить параметр" );
    m_ptBtnInc->setFixedWidth( 93 );
    //setIncBtnVisible( false );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout, 0 );

    setLayout( m_vlayout );

    // ставим начальный размер себя
    widget_size_reset();

    widget_stretch( m_vlayout->minimumSize().width(), m_vlayout->minimumSize().height() );
}

TCategories::~TCategories()
{

}

void TCategories::clear()
{
    m_zName.clear();
    m_zBtnName.clear();

    m_zUlink.clear();
    m_zUname.clear();

    m_apParamList.clear();

    need_to_add = false;
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

    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgName( "NewParameter" );

    m_ptDialog->open();
}

void  TCategories::onBtnName()
{
//    qDebug() << getCategoriesName() << "button";

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgEnabled( false );

    m_ptDialog->setDlgUlinkEnabled( true );
    m_ptDialog->setDlgUnameEnabled( true );

    m_ptDialog->setDlgName( getCategoriesName() );
    m_ptDialog->setDlgUlink( getCategoriesUlink() );
    m_ptDialog->setDlgUname( getCategoriesUname() );

    m_ptDialog->open();
}

void  TCategories::onSendCancel()
{
    need_to_add = false;
}

void  TCategories::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    if( false == need_to_add )
    {
        // редактируем текущий набор параметров

        setCategoriesName( m_tValues.m_zName.toStdString(), true );
        setCategoriesName( m_tValues.m_zUlink.toStdString(), true );
        setCategoriesName( m_tValues.m_zUname.toStdString(), true );

        //qDebug() << getCategoriesName() << "fix categories";
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров

        TParam  *pParam;
        pParam = new TParam( Q_NULLPTR, this, m_depth+1 );
        pParam->setNode( m_node[ GoodsParametersSection ] );

        //!bug  надо перерисовывать все layout-ы
        // добавляемся к родителю
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

        // создаем пустой узел
        YAML::Node  node;
        node.reset();

        // пишем в узел значения параметров
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

        // добалляем узел в ямл
        m_node[ GoodsParametersSection ].push_back( node );

        //qDebug() << pParam->getParamName() << "add parameter";
    }

    need_to_add = false;
}

//------------------------------------------------------------------------------

void  TCategories::clearNodeSequence()
{
    // если не знаем индекс, то удаляем поля
    if( -1 == m_node_index )
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
    }

    // если знаем индекс, то удаляем всю ветку
    if( -1 != m_node_index )
    {
        m_node_parent.remove( m_node_index );
    }
}

void  TCategories::clearNodeParameters()
{
    m_node.remove( GoodsParametersSection );
}

void  TCategories::CategoriesDelete()
{
    QLayoutItem *child;
    QString  item = getCategoriesName();

    //qDebug() << "categories" << item << "delete";

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

    widget_parent_shrink( 0, getCategoriesHeight() );

    // удаляем виджеты на первой строке
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
    if( Q_NULLPTR != m_pMentor )
    {
        for( int i = 0; i < m_pMentor->m_apCategoriesList.count(); i++ )
        {
            if( this == m_pMentor->m_apCategoriesList.at(i) )
            {
                //qDebug() << m_pMentor->m_apCategoriesList.at(i)->getCategoriesName() << "obsolete";

                m_pMentor->m_apCategoriesList.removeAt(i);
            }
        }

        // удаляем запись в values родителя
        m_pMentor->remParamList( item, true );
    }

    // очищаем вложения
    clearNodeParameters();

    // очищаем ветку
    clearNodeSequence();
}

//------------------------------------------------------------------------------

void  TCategories::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TCategories::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TCategories::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TCategories::getNode()
{
    return m_node;
}

void  TCategories::setCategoriesName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Категория: " + m_zName );  // подсказка с оригинальным именем

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsNameSection, name );
    }
}

void  TCategories::setCategoriesUlink( const std::string&  name, bool  set_to_node )
{
    m_zUlink = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsUlinkSection, name );
    }
}

void  TCategories::setCategoriesUname( const std::string&  name, bool  set_to_node )
{
    m_zUname = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsUnameSection, name );
    }
}

QString  TCategories::getCategoriesName()
{
    return m_zName;
}

QString  TCategories::getCategoriesUlink()
{
    return m_zUlink;
}

QString  TCategories::getCategoriesUname()
{
    return m_zUname;
}

void  TCategories::setCategoriesDepth( int  depth )
{
    m_depth = depth;
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

void  TCategories::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;

    m_width = 2 * m_vlayout->margin();
    m_height = 2 * m_vlayout->margin();

    //qDebug() << "init categories size" << m_width << m_height;

    // ставим размер самого себя
//    setMinimumWidth( m_width );
//    setMinimumHeight( m_height );
}

void  TCategories::widget_stretch( int width, int height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    qDebug() << "categories"  << width << height << m_width << m_height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );
}

void  TCategories::widget_parent_stretch( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pMentor )
    {
        m_pMentor->widget_stretch( width, height );
        m_pMentor->widget_parent_stretch( width, height );
    }
}

void  TCategories::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumHeight( m_height );
}

void  TCategories::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pMentor )
    {
        m_pMentor->widget_shrink( width, height );
        m_pMentor->widget_parent_shrink( width, height );
    }
}

int TCategories::getCategoriesWidth()
{
    return m_vlayout->minimumSize().width();
}

int TCategories::getCategoriesHeight()
{
    return m_vlayout->minimumSize().height();
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

    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    m_hlayout1 = new QHBoxLayout;
    m_hlayout1->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth; i++ )
    {
        QLabel *label = new QLabel();
        label->setFixedWidth( 93 );
        m_hlayout1->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-" );
    m_ptBtnDec->setToolTip( "Удалить параметр" );
    m_ptBtnDec->setFixedWidth( 93 );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    m_hlayout1->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "button" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, SIGNAL(clicked()), this, SLOT(onBtnName()) );
    m_hlayout1->addWidget( m_ptBtnName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить категорию" );
    m_ptBtnInc->setFixedWidth( 93 );
    setIncBtnVisible( false );  // по умолчанию кнопка невидимая
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_hlayout1->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout1, 0 );

    setLayout( m_vlayout );

    // ставим начальный размер себя
    widget_size_reset();

    widget_stretch( m_vlayout->minimumSize().width(), m_vlayout->minimumSize().height() );
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
//    qDebug() << getParamName() << "dec val button";

    setParamType( 0, true );
}

void  TParam::onBtnInc()
{
//    qDebug() << getParamName() << "inc button";

    // признак что хотим создать новую категорию
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialog->setDlgEmpty();

    m_ptDialog->setDlgEnabled( false );

    m_ptDialog->setDlgUlinkEnabled( true );
    m_ptDialog->setDlgUnameEnabled( true );

    m_ptDialog->setDlgName( "NewCategory" );

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

        //qDebug() << getParamName() << "fix parameter";
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров

        // тут мы должны добавить поле field в values и создать categories

        // добавляем новое поле в values
        addParamList( m_tValues.m_zName, true );

        // добавляем категорию
        TCategories  *pCategories;
        pCategories = new TCategories( this, m_depth+1 );
        pCategories->setNode( m_node[ GoodsCategoriesSection ] );

        //!bug  надо перерисовывать все layout-ы
        // добавляемся к родителю
        //m_vlayout->insertWidget( 1, pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );
        m_apCategoriesList.append( pCategories );

        // ставим значения параметров
        pCategories->setCategoriesName( m_tValues.m_zName.toStdString() );
        pCategories->setCategoriesUlink( m_tValues.m_zUlink.toStdString() );
        pCategories->setCategoriesUname( m_tValues.m_zUname.toStdString() );

        widget_stretch( pCategories->getCategoriesWidth(), pCategories->getCategoriesHeight() );
        widget_parent_stretch( pCategories->getCategoriesWidth(), pCategories->getCategoriesHeight() );

        // создаем пустой узел
        YAML::Node  node;
        node.reset();

        // пишем в узел значения параметров
        __yaml_SetString( node, GoodsNameSection, m_tValues.m_zName.toStdString() );
        __yaml_SetString( node, GoodsUlinkSection, m_tValues.m_zUlink.toStdString() );
        __yaml_SetString( node, GoodsUnameSection, m_tValues.m_zUname.toStdString() );

        // добаляем узел в ямл
        m_node[ GoodsCategoriesSection ].push_back( node );

        //qDebug() << pCategories->getCategoriesName() << "add categories";
    }

    need_to_add = false;
}

void  TParam::onSendValue( int  val )
{
    m_tValues.m_uValue = static_cast<unsigned>(val);
}

//------------------------------------------------------------------------------

void  TParam::clearNodeSequence()
{
    // если не знаем индекс, то удаляем поля
    if( -1 == m_node_index )
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
    }

    // если знаем индекс, то удаляем всю ветку
    if( -1 != m_node_index )
    {
        m_node_parent.remove( m_node_index );
    }
}

void  TParam::clearNodeCategories()
{
    // удаляем ветку categories
    m_node.remove( GoodsCategoriesSection );
}

void  TParam::ParamDelete()
{
    QLayoutItem *child;

    qDebug() << "parameter" << getParamName() << "delete";

    // уничтожаем диалог
    m_ptDialog->~TDialog();

    // для всех вложенных Categories вызываем очистку
    for( auto& it : m_apCategoriesList )
    {
        // очищаем
        it->CategoriesDelete();

        // уничтожаем
        it->~TCategories();
    }

    widget_parent_shrink( 0, getParamHeight() );

    // удаляем виджеты на второй строке
    if( m_second_row_exist )
    {
        while( ( child = m_hlayout2->takeAt(0) ) != Q_NULLPTR )
        {
            delete child->widget();
            delete child;
        }
    }

    // удаляем виджеты на первой строке
    while( ( child = m_hlayout1->takeAt(0) ) != Q_NULLPTR )
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
    if( Q_NULLPTR != m_pMentor )
    {
        // родитель

        for( int i = 0; i < m_pMentor->m_apParamList.count(); i++ )
        {
            if( this == m_pMentor->m_apParamList.at(i) )
            {
                //qDebug() << m_pMentor->m_apParamList.at(i)->getParamName() << "obsolete (mentor)";

                m_pMentor->m_apParamList.removeAt(i);
            }
        }
    }
    else if( Q_NULLPTR != m_pAncestor )
    {
        // прародитель

        for( int i = 0; i < m_pAncestor->m_apParamList.count(); i++ )
        {
            if( this == m_pAncestor->m_apParamList.at(i) )
            {
                //qDebug() << m_pAncestor->m_apParamList.at(i)->getParamName() << "obsolete (ancestor)";

                m_pAncestor->m_apParamList.removeAt(i);
            }
        }
    }

    // очищаем вложения
    clearNodeCategories();

    // очищаем ветку в ямле
    clearNodeSequence();
}

void  TParam::ParamDraw( TParam  *a_pParam )
{
    m_vlayout->addWidget( a_pParam, 0, Qt::AlignLeft | Qt::AlignTop );
}

//------------------------------------------------------------------------------

void  TParam::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TParam::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TParam::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TParam::getNode()
{
    return m_node;
}

void  TParam::setParamName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Параметр: " +  m_zName );  // подсказка с оригинальным именем

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

//    qDebug() << getParamName() << "set param" << m_uType << m_second_row_exist;

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

void  TParam::remParamList( QString& item, bool  set_to_node )
{
    // убираем запись из списка
    for( auto& it : m_vList )
    {
        if( item.simplified() == it.simplified() )
        {
            m_vList.removeAt( m_vList.indexOf(it) );
            break;
        }
    }

    int  ind = 0;
    m_zList.clear();

    // формируем новую строку
    for( auto& it : m_vList )
    {
        if( 0 != ind )
            m_zList.append( "\n" );

        m_zList.append( it.toStdString() );

        ind++;
    }

    //qDebug() << "новая строка values:" << QString::fromStdString(m_zList);

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
    }
}

void  TParam::addParamList( QString& item, bool  set_to_node )
{
    // добавляем запись в строку
    if( 0 == m_zList.length() )
    {
        // если строка списка пустая, то просто добавляем
        m_zList.append( item.toStdString() );
    }
    else
    {
        // если строка списка не пустая, то добавляем с префиксом
        m_zList.append( "\n" );
        m_zList.append( item.toStdString() );
    }

    // добавляем запись в список
    m_vList.append( item );

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
    }
}

void  TParam::setParamList( QStringList  vlist, const std::string&  zlist, bool  set_to_node )
{
    m_vList = vlist;

    m_zList = zlist;

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
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

void  TParam::setParamDepth( int  depth )
{
    m_depth = depth;
}

int  TParam::getParamDepth()
{
    return m_depth;
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

    m_hlayout2 = new QHBoxLayout();
    m_hlayout2->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // отступ
    for( int i = 0; i < m_depth+1; i++ )
    {
        QLabel *label = new QLabel();
        label->setFixedWidth( 93 );
        m_hlayout2->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnValDec = new QPushButton( "-" );
    m_ptBtnValDec->setToolTip( "Удалить значение" );
    m_ptBtnValDec->setFixedWidth( 93 );
    connect( m_ptBtnValDec, SIGNAL(clicked()), this, SLOT(onBtnValDec()) );
    m_hlayout2->addWidget( m_ptBtnValDec, 0, Qt::AlignLeft );

    // спин со значением
    m_ptSpinValue = new QSpinBox();
    m_ptSpinValue->setToolTip( "Значение" );
    m_ptSpinValue->setFixedWidth( 93 );
    connect( m_ptSpinValue, SIGNAL(valueChanged(int)), this, SLOT(onSendValue(int)) );
    m_hlayout2->addWidget( m_ptSpinValue, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout2 );

    //qDebug() << "add value" << m_hlayout->minimumSize().width() << m_hlayout->minimumSize().height();

    widget_stretch( m_hlayout2->minimumSize().width(), m_hlayout2->minimumSize().height() );
    widget_parent_stretch( m_hlayout2->minimumSize().width(), m_hlayout2->minimumSize().height() );

    m_second_row_exist = true;
}

void  TParam::setParamValueDel()
{
    if( !m_second_row_exist )
        return;

    QLayoutItem *child;

    // уничтожаем виджеты
    while( ( child = m_hlayout2->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    //qDebug() << "del value" << m_hlayout->minimumSize().width() << m_hlayout->minimumSize().height();

    widget_shrink( 0, m_hlayout2->minimumSize().height() );
    widget_parent_shrink( 0, m_hlayout2->minimumSize().height() );

    // уничтожаем layout
    m_hlayout2->deleteLater();

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

void  TParam::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;

    m_width = 2 * m_vlayout->margin();
    m_height = 2 * m_vlayout->margin();

    //qDebug() << "init param size" << m_width << m_height;

    // ставим размер самого себя
    //setMinimumWidth( m_width );
    //setMinimumHeight( m_height );
}

void  TParam::widget_stretch( int width, int height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height += height;

//    qDebug() << "parameter" << getParamName() << m_width << m_height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );
}

void  TParam::widget_parent_stretch( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_stretch( width, height );
    }
    else if( Q_NULLPTR != m_pMentor )
    {
        m_pMentor->widget_stretch( width, height );
        m_pMentor->widget_parent_stretch( width, height );
    }
}

void  TParam::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumHeight( m_height );
}

void  TParam::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        qDebug() << "ancestor shrink";
        m_pAncestor->widget_shrink( width, height );
        m_pAncestor->widget_parent_shrink( width, height );
    }
    else if( Q_NULLPTR != m_pMentor )
    {
        qDebug() << "mentor shrink";
        m_pMentor->widget_shrink( width, height );
        m_pMentor->widget_parent_shrink( width, height );
    }
}

int TParam::getParamWidth()
{
    return m_vlayout->minimumSize().width();
}

int TParam::getParamHeight()
{
    return m_vlayout->minimumSize().height();
}

//------------------------------------------------------------------------------
