#include "func.h"
#include "param.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategories::TCategories( TParam  *pMentor, int  depth )
{
    clear();

    // диалог для себя
    m_ptDialogSelf = new TDialog( false, "Value",  this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialogSelf, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialogSelf, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    // диалог для добавления параметра
    m_ptDialogAdd = new TDialog( true, "Add parameter",  this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialogAdd, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialogAdd, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

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
    m_hlayout->setMargin( 0 );

    // отступ
    for( int i = 0; i < m_depth; i++ )
    {
        QLabel *label = new QLabel();
        label->setFixedWidth( 93 );
        m_hlayout->addWidget( label, 0, Qt::AlignLeft );
    }

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-" );
    m_ptBtnDec->setToolTip( "Удалить значение" );
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

    widget_size_reset();

    int  width = 2*m_vlayout->margin() + 3*m_ptBtnInc->minimumSize().width() + 2*m_vlayout->spacing() + m_depth*( m_ptBtnInc->minimumSize().width() + m_vlayout->spacing() );
    int  height = 2*m_vlayout->margin() + m_ptBtnInc->minimumSizeHint().height();

    widget_stretch( width, height );
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
    widget_shrink( getCategoriesWidth(), getCategoriesHeight() + m_vlayout->spacing() );

    CategoriesDelete();
}

void  TCategories::onBtnInc()
{
    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialogAdd->setDlgEmpty();

    m_ptDialogAdd->setDlgName( "NewParameter" );

    m_ptDialogAdd->open();
}

void  TCategories::onBtnName()
{
    // диалог с пустыми параметрами
    m_ptDialogSelf->setDlgEmpty();

    m_ptDialogSelf->setDlgName( getCategoriesName() );
    m_ptDialogSelf->setDlgUlink( getCategoriesUlink() );
    m_ptDialogSelf->setDlgUname( getCategoriesUname() );

    m_ptDialogSelf->open();
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
        setCategoriesUlink( m_tValues.m_zUlink.toStdString(), true );
        setCategoriesUname( m_tValues.m_zUname.toStdString(), true );

        //qDebug() << getCategoriesName() << "fix categories";
    }

    if( true == need_to_add )
    {
        // создаем новый набор параметров

        TParam  *pParam;
        pParam = new TParam( Q_NULLPTR, this, m_depth+1 );

        // добавляемся к родителю
        //m_vlayout->insertWidget( 1, pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_apParamList.append( pParam );

        // ставим значения параметров
        pParam->setParamName( m_tValues.m_zName.toStdString() );
        pParam->setParamType( m_tValues.m_uType );

        if( 0 != m_tValues.m_uType )
        {
            pParam->setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString() );
            pParam->setParamNew( m_tValues.m_zNew.toStdString() );
            pParam->setParamAfter( m_tValues.m_zAfter.toStdString() );
            pParam->setParamBefore( m_tValues.m_zBefore.toStdString() );
            pParam->setParamUlink( m_tValues.m_zUlink.toStdString() );
            pParam->setParamUname( m_tValues.m_zUname.toStdString() );
        }

        if( 2 == m_tValues.m_uType )
        {
            pParam->setParamMin( m_tValues.m_nMin );
            pParam->setParamMax( m_tValues.m_nMax );
        }
        else if( 3 == m_tValues.m_uType )
        {
            pParam->setParamDMin( m_tValues.m_fMin );
            pParam->setParamDMax( m_tValues.m_fMax );
        }

        if( 5 == m_tValues.m_uType )
        {
            pParam->setParamMulti( m_tValues.m_zMulti.toStdString() );
        }

        YAML::Node  node;
        node.reset();

        // пишем их в пустой ямл
        __yaml_SetString( node, GoodsNameSection, m_tValues.m_zName.toStdString() );
        __yaml_SetScalar( node, GoodsTypeSection, m_tValues.m_uType );

        if( 0 != m_tValues.m_uType )
        {
            __yaml_SetString( node, GoodsPlaceholderSection, m_tValues.m_zPlaceholder.toStdString() );
            __yaml_SetString( node, GoodsNewSection, m_tValues.m_zNew.toStdString() );
            __yaml_SetString( node, GoodsAfterSection, m_tValues.m_zAfter.toStdString() );
            __yaml_SetString( node, GoodsBeforeSection, m_tValues.m_zBefore.toStdString() );
            __yaml_SetString( node, GoodsUlinkSection, m_tValues.m_zUlink.toStdString() );
            __yaml_SetString( node, GoodsUnameSection, m_tValues.m_zUname.toStdString() );
        }

        if( 2 == m_tValues.m_uType )
        {
            __yaml_SetInteger( node, GoodsMinSection, m_tValues.m_nMin );
            __yaml_SetInteger( node, GoodsMaxSection, m_tValues.m_nMax );
        }
        else if( 3 == m_tValues.m_uType )
        {
            __yaml_SetDouble( node, GoodsMinSection, m_tValues.m_fMin );
            __yaml_SetDouble( node, GoodsMaxSection, m_tValues.m_fMax );
        }

        if( 5 == m_tValues.m_uType )
        {
            __yaml_SetString( node, GoodsMultiSection, m_tValues.m_zMulti.toStdString() );
        }

        // добавляем ямл к основному
        m_node[ GoodsParametersSection ].push_back( node );

        int index = static_cast<int>(m_node[ GoodsParametersSection ].size()) - 1;

        pParam->setNode( m_node[ GoodsParametersSection ][index] );
        pParam->setNodeParent( m_node[ GoodsParametersSection ] );
        pParam->setNodeIndex( index );

        pParam->setParamNameColor();

        //qDebug() << pParam->getParamName() << "index" << index;
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
        //m_node.remove( GoodsTypeSection );
        //m_node.remove( GoodsPlaceholderSection );
        //m_node.remove( GoodsNewSection );
        //m_node.remove( GoodsAfterSection );
        //m_node.remove( GoodsBeforeSection );
        m_node.remove( GoodsUlinkSection );
        m_node.remove( GoodsUnameSection );
        //m_node.remove( GoodsMinSection );
        //m_node.remove( GoodsMaxSection );
        //m_node.remove( GoodsMultiSection );
        //m_node.remove( GoodsValuesSection );
    }

    // если знаем индекс, то удаляем всю ветку
    if( -1 != m_node_index )
    {
        m_node_parent.remove( m_node_index );
    }
}

void  TCategories::CategoriesDelete()
{
    QLayoutItem *child;
    QString  item = getCategoriesName();

    // для всех вложенных Parameters вызываем очистку
    for( auto& it : m_apParamList )
    {
        // очищаем
        it->ParamDelete();

        // удаляем
        delete it;
    }

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
        // удаляемся из списка детей
        for( int i = 0; i < m_pMentor->m_apCategoriesList.count(); i++ )
        {
            if( this == m_pMentor->m_apCategoriesList.at(i) )
            {
                m_pMentor->m_apCategoriesList.removeAt(i);

                break;
            }
        }

        // удаляем запись в values родителя
        m_pMentor->remParamList( item, true );

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_pMentor->m_apCategoriesList.count(); i++ )
        {
            index = m_pMentor->m_apCategoriesList.at(i)->getNodeIndex();

            if( index > m_node_index )
            {
                index -= 1;

                m_pMentor->m_apCategoriesList.at(i)->setNodeIndex( index );
            }
        }

        // удаляемся из родительского layout-а
        m_pMentor->m_vlayout->removeWidget(this);
    }

    // очищаем ветку
    clearNodeSequence();

    if( Q_NULLPTR != m_pMentor )
    {
        // если нет категорий, то удаляем строку "categories:"
        if( m_pMentor->m_apCategoriesList.isEmpty() )
        {
            m_pMentor->getNode().remove( GoodsCategoriesSection );
        }
    }
}

//------------------------------------------------------------------------------

bool  TCategories::isParamNameRedefined( const QString&  name )
{
    if( Q_NULLPTR != m_pMentor )
    {
        return m_pMentor->isParamNameRedefined( name );
    }

    return false;
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

YAML::Node&  TCategories::getNodeParent()
{
    return m_node_parent;
}

int  TCategories::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TCategories::setCategoriesName( const std::string&  name, bool  set_to_node )
{
    int  height;

    m_zName = QString::fromStdString(name);

    height = m_ptBtnName->minimumSizeHint().height();

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Значение: " + m_zName );  // подсказка с оригинальным именем

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
}

void  TCategories::widget_stretch( int width, int height, bool add_height ) noexcept
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

void  TCategories::widget_parent_stretch( int width, int height, bool add_height ) noexcept
{
    int  val = 0;

    if( Q_NULLPTR != m_pMentor )
    {
        if( add_height )
        {
            val = m_pMentor->m_vlayout->spacing();
        }

        m_pMentor->widget_stretch( width, height + val, false );
    }
}

void  TCategories::widget_shrink( int width, int height ) noexcept
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

void  TCategories::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pMentor )
    {
        m_pMentor->widget_shrink( width, height );
    }
}

int TCategories::getCategoriesWidth() noexcept
{
    return minimumSize().width();
}

int TCategories::getCategoriesHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

TParam::TParam( TCategory  *pAncestor, TCategories  *pMentor, int  depth )
{
    clear();

    // диалог для себя
    m_ptDialogSelf = new TDialog( true, "Parameter",  this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialogSelf, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialogSelf, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

    // диалог для добавления values
    m_ptDialogAdd = new TDialog( false, "Add value",  this );

    // ловим сигнал от диалога об отмене
    connect( m_ptDialogAdd, SIGNAL(sendCancel()), this, SLOT(onSendCancel()) );

    // ловим сигнал от диалога с данными
    connect( m_ptDialogAdd, SIGNAL(sendValues(TValues&)), this, SLOT(onSendValues(TValues&)) );

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
    m_hlayout1->setMargin( 0 );

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
    m_ptBtnInc->setToolTip( "Добавить значение" );
    m_ptBtnInc->setFixedWidth( 93 );
    setIncBtnVisible( false );  // по умолчанию кнопка невидимая
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_hlayout1->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( m_hlayout1, 0 );

    setLayout( m_vlayout );

    widget_size_reset();

    int  width = 2*m_vlayout->margin() + 3*m_ptBtnInc->minimumSize().width() + 2*m_vlayout->spacing() + m_depth*( m_ptBtnInc->minimumSize().width() + m_vlayout->spacing() );
    int  height = 2*m_vlayout->margin() + m_ptBtnInc->minimumSizeHint().height();

    widget_stretch( width, height );
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
    m_nMin = 0;
    m_nMax = 0;
    m_fMin = 0.0;
    m_fMax = 0.0;

    m_vList.clear();

    m_apCategoriesList.clear();

    need_to_add = false;

    m_second_row_exist = false;
    m_second_row_type = 0;
}

//------------------------------------------------------------------------------

void  TParam::onBtnDec()
{
    widget_shrink( getParamWidth(), getParamHeight() + m_vlayout->spacing() );

    ParamDelete();
}

void  TParam::onBtnValDec()
{
    setParamType( 0, true );
}

void  TParam::onBtnInc()
{
    // признак что хотим создать новую категорию
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptDialogAdd->setDlgEmpty();

    m_ptDialogAdd->setDlgName( "NewValue" );

    m_ptDialogAdd->open();
}

void  TParam::onBtnName()
{
    // диалог с пустыми параметрами
    m_ptDialogSelf->setDlgEmpty();

    m_ptDialogSelf->setDlgName( getParamName() );
    m_ptDialogSelf->setDlgPlaceholder( getParamPlaceholder() );
    m_ptDialogSelf->setDlgNew( getParamNew() );
    m_ptDialogSelf->setDlgAfter( getParamAfter() );
    m_ptDialogSelf->setDlgBefore( getParamBefore() );
    m_ptDialogSelf->setDlgUlink( getParamUlink() );
    m_ptDialogSelf->setDlgUname( getParamUname() );
    m_ptDialogSelf->setDlgMulti( getParamMulti() );

    m_ptDialogSelf->setDlgType( getParamType() );

    m_ptDialogSelf->setDlgMin( getParamMin() );
    m_ptDialogSelf->setDlgMax( getParamMax() );

    m_ptDialogSelf->setDlgDMin( getParamDMin() );
    m_ptDialogSelf->setDlgDMax( getParamDMax() );

    m_ptDialogSelf->setDlgCombo( getParamList() );

    m_ptDialogSelf->setDlgTypeEnabled( isChildrenAbsent() );

    m_ptDialogSelf->open();
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

        if( 0 != m_tValues.m_uType )
        {
            setParamPlaceholder( m_tValues.m_zPlaceholder.toStdString(), true );
            setParamNew( m_tValues.m_zNew.toStdString(), true );
            setParamAfter( m_tValues.m_zAfter.toStdString(), true );
            setParamBefore( m_tValues.m_zBefore.toStdString(), true );
            setParamUlink( m_tValues.m_zUlink.toStdString(), true );
            setParamUname( m_tValues.m_zUname.toStdString(), true );
        }

        if( 2 == m_tValues.m_uType )
        {
            setParamMin( m_tValues.m_nMin, true );
            setParamMax( m_tValues.m_nMax, true );
        }
        else if( 3 == m_tValues.m_uType )
        {
            setParamDMin( m_tValues.m_fMin, true );
            setParamDMax( m_tValues.m_fMax, true );
        }

        if( 5 == m_tValues.m_uType )
        {
            setParamMulti( m_tValues.m_zMulti.toStdString(), true );
        }

        setParamNameColor();

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

        //!bug  надо перерисовывать все layout-ы
        // добавляемся к родителю
        //m_vlayout->insertWidget( 1, pParam, 0, Qt::AlignLeft | Qt::AlignTop );
        m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );
        m_apCategoriesList.append( pCategories );

        // ставим значения параметров
        pCategories->setCategoriesName( m_tValues.m_zName.toStdString() );
        pCategories->setCategoriesUlink( m_tValues.m_zUlink.toStdString() );
        pCategories->setCategoriesUname( m_tValues.m_zUname.toStdString() );

        YAML::Node  node;
        node.reset();

        // пишем их в пустой ямл
        __yaml_SetString( node, GoodsNameSection, m_tValues.m_zName.toStdString() );
        __yaml_SetString( node, GoodsUlinkSection, m_tValues.m_zUlink.toStdString() );
        __yaml_SetString( node, GoodsUnameSection, m_tValues.m_zUname.toStdString() );

        // добавляем ямл к основному
        m_node[ GoodsCategoriesSection ].push_back( node );

        int index = static_cast<int>(m_node[ GoodsCategoriesSection ].size()) - 1;

        pCategories->setNode( m_node[ GoodsCategoriesSection ][index] );
        pCategories->setNodeParent( m_node[ GoodsCategoriesSection ] );
        pCategories->setNodeIndex( index );

        //qDebug() << pCategories->getCategoriesName() << "index" << index;
    }

    need_to_add = false;
}

void  TParam::onSendValue( QString  val )
{
    m_tValues.m_zVal = val;
}

void  TParam::onSendValue( int  val )
{
    m_tValues.m_nVal = val;
}

void  TParam::onSendValue( double  val )
{
    m_tValues.m_fVal = val;
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

void  TParam::ParamDelete()
{
    QLayoutItem *child;

    // для всех вложенных Categories вызываем очистку
    for( auto& it : m_apCategoriesList )
    {
        // очищаем
        it->CategoriesDelete();

        // удаляем
        delete it;
    }

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

    // очищаем ветку в ямле
    clearNodeSequence();

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pMentor )
    {
        // родитель

        // удаляемся из списка детей
        for( int i = 0; i < m_pMentor->m_apParamList.count(); i++ )
        {
            if( this == m_pMentor->m_apParamList.at(i) )
            {
                m_pMentor->m_apParamList.removeAt(i);

                break;
            }
        }

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_pMentor->m_apParamList.count(); i++ )
        {
            index = m_pMentor->m_apParamList.at(i)->getNodeIndex();

            if( index > m_node_index )
            {
                index -= 1;

                m_pMentor->m_apParamList.at(i)->setNodeIndex( index );
            }
        }

        // если нет параметров, то удаляем строку "parameters:"
        if( m_pMentor->m_apParamList.isEmpty() )
        {
            m_pMentor->getNode().remove( GoodsParametersSection );
        }

        // удаляемся из родительского layout-а
        m_pMentor->m_vlayout->removeWidget(this);
    }
    else if( Q_NULLPTR != m_pAncestor )
    {
        // прародитель

        // удаляемся из списка детей
        for( int i = 0; i < m_pAncestor->m_apParamList.count(); i++ )
        {
            if( this == m_pAncestor->m_apParamList.at(i) )
            {
                m_pAncestor->m_apParamList.removeAt(i);

                break;
            }
        }

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_pAncestor->m_apParamList.count(); i++ )
        {
            index = m_pAncestor->m_apParamList.at(i)->getNodeIndex();

            if( index > m_node_index )
            {
                index -= 1;

                m_pAncestor->m_apParamList.at(i)->setNodeIndex( index );
            }
        }

        // если нет параметров, то удаляем строку "parameters:"
        if( m_pAncestor->m_apParamList.isEmpty() )
        {
            m_pAncestor->getNode().remove( GoodsParametersSection );
        }

        // удаляемся из родительского layout-а
        m_pAncestor->m_vlayout->removeWidget(this);
    }
}

//------------------------------------------------------------------------------

// метод не вызывается напрямую, вызов идет через метод родителя isParamNameRedefined
bool  TParam::isParamNameRedefined( const QString&  name )
{
    QList<TParam*> list;

    list.clear();

    // берем список параметров у родителя
    if( Q_NULLPTR != m_pMentor )
    {
        qDebug() << "_mentor name" << m_pMentor->getCategoriesName();

        list = m_pMentor->m_apParamList;
    }
    else if( Q_NULLPTR != m_pAncestor )
    {
        qDebug() << "_ancestor name" << m_pAncestor->getCategoryName();

        list = m_pAncestor->m_apParamList;
    }

    // если список не пуст, то просматриваем его
    if( list.size() )
    {
        for( auto& it : list )
        {
            // нашли совпадение -> сообщаем об этом
            if( isStrEqual( name, it->getParamName() ) )
            {
                return true;
            }
        }
    }

    return false;
}

//------------------------------------------------------------------------------

bool  TParam::isChildrenAbsent()
{
    return m_apCategoriesList.isEmpty();
}

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

YAML::Node&  TParam::getNodeParent()
{
    return m_node_parent;
}

int  TParam::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TParam::setParamName( const std::string&  name, bool  set_to_node )
{
    int  height;

    m_zName = QString::fromStdString(name);

    height = m_ptBtnName->minimumSizeHint().height();

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Параметр: " +  m_zName );  // подсказка с оригинальным именем

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

    if( ( 1 == m_uType ) || ( 2 == m_uType ) || ( 3 == m_uType ) )
    {
        setParamValueAdd( m_uType );
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

void  TParam::setParamMin( int  val, bool  set_to_node )
{
    m_nMin = val;

    setParamValueMin( m_nMin );

    if( set_to_node )
    {
        __yaml_SetInteger( m_node, GoodsMinSection, val );
    }
}

void  TParam::setParamMax( int  val, bool  set_to_node )
{
    m_nMax = val;

    setParamValueMax( m_nMax );

    if( set_to_node )
    {
        __yaml_SetInteger( m_node, GoodsMaxSection, val );
    }
}

void  TParam::setParamDMin( double  val, bool  set_to_node )
{
    m_fMin = val;

    setParamValueDMin( m_fMin );

    if( set_to_node )
    {
        __yaml_SetDouble( m_node, GoodsMinSection, val );
    }
}

void  TParam::setParamDMax( double  val, bool  set_to_node )
{
    m_fMax = val;

    setParamValueDMax( m_fMax );

    if( set_to_node )
    {
        __yaml_SetDouble( m_node, GoodsMaxSection, val );
    }
}

//------------------------------------------------------------------------------

bool  TParam::isStrEqual( QString  str1, QString  str2 )
{
    // убираем пробелы
    str1.replace( " ", "" );
    str2.replace( " ", "" );

    // сравниваем без учета регистра
    if( 0 == QString::compare( str1, str2, Qt::CaseInsensitive ) )
        return true;

    return false;
}

void  TParam::colorBtnName( bool  color )
{
    if( color )
    {
        if( 0 == getParamType() )
        {
            m_ptBtnName->setStyleSheet( "color: red" );
        }
        else
        {
            m_ptBtnName->setStyleSheet( "color: green" );
        }
    }
    else
    {
        m_ptBtnName->setStyleSheet( "color: default" );
    }
}

void  TParam::setParamNameColor( bool  force )
{
    bool  mentor = false;
    QList<TParam*> list;

    list.clear();

    colorBtnName( false );

    // берем список параметров у родителя
    if( Q_NULLPTR != m_pMentor )
    {
        qDebug() << "mentor name" << m_pMentor->getCategoriesName();

        list = m_pMentor->m_apParamList;

        mentor = true;
    }
    else if( Q_NULLPTR != m_pAncestor )
    {
        qDebug() << "ancestor name" << m_pAncestor->getCategoryName();

        list = m_pAncestor->m_apParamList;
    }

    // если список не пуст, то просматриваем его
    if( list.size() )
    {
        for( auto& it : list )
        {
            // смотрим только параметры с индексами меньше чем у нас
            if( ( force ) || ( it->getNodeIndex() < getNodeIndex() ) )
            {
                qDebug() << "parent ind" << it->getNodeIndex() << "ind" << getNodeIndex() ;

                // проверяем у родителя
                // нашли совпадение -> красим
                if( isStrEqual( getParamName(), it->getParamName() ) )
                {
                    qDebug() << "color by parent" << it->getParamName() << getParamName();
                    colorBtnName( true );

//                    break;
                }

                // проверяем у родителя родителя
                // для ancestor вызов не делаем
                // нашли совпадение -> красим
                if( mentor )
                if( it->isParamNameRedefined( getParamName() ) )
                {
                    qDebug() << "color by parent of parent";
                    colorBtnName( true );
                }
            }
        }
    }
}

//------------------------------------------------------------------------------

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

QString  TParam::getParamName() noexcept
{
    return m_zName;
}

QString  TParam::getParamPlaceholder() noexcept
{
    return m_zPlaceholder;
}

QString  TParam::getParamNew() noexcept
{
    return m_zNew;
}

QString  TParam::getParamAfter() noexcept
{
    return m_zAfter;
}

QString  TParam::getParamBefore() noexcept
{
    return m_zBefore;
}

QString  TParam::getParamUlink() noexcept
{
    return m_zUlink;
}

QString  TParam::getParamUname() noexcept
{
    return m_zUname;
}

QString  TParam::getParamMulti() noexcept
{
    return m_zMulti;
}


unsigned  TParam::getParamType() noexcept
{
    return m_uType;
}

int  TParam::getParamMin() noexcept
{
    return m_nMin;
}

int  TParam::getParamMax() noexcept
{
    return m_nMax;
}

double  TParam::getParamDMin() noexcept
{
    return m_fMin;
}

double  TParam::getParamDMax() noexcept
{
    return m_fMax;
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

void  TParam::setParamValueAdd( unsigned  type )
{
    if( m_second_row_exist )
    {
        setParamValueChange( type );

        return;
    }

    m_hlayout2 = new QHBoxLayout();
    m_hlayout2->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_hlayout2->setMargin( 0 );

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

    // поле со значением
    m_ptLineValue = new QLineEdit();
    m_ptLineValue->setToolTip( "Значение" );
    m_ptLineValue->setFixedWidth( 93 );
    connect( m_ptLineValue, SIGNAL(textChanged(QString)), this, SLOT(onSendValue(QString)) );

    // спин со значением
    m_ptSpinValue = new QSpinBox();
    m_ptSpinValue->setToolTip( "Значение" );
    m_ptSpinValue->setFixedWidth( 93 );
    connect( m_ptSpinValue, SIGNAL(valueChanged(int)), this, SLOT(onSendValue(int)) );

    // дабл спин со значением
    m_ptDSpinValue = new QDoubleSpinBox();
    m_ptDSpinValue->setToolTip( "Значение" );
    m_ptDSpinValue->setFixedWidth( 93 );
    m_ptDSpinValue->setDecimals( 1 );
    connect( m_ptDSpinValue, SIGNAL(valueChanged(double)), this, SLOT(onSendValue(double)) );

    m_second_row_type = type;

    if( 1 == type )
    {
        m_hlayout2->addWidget( m_ptLineValue, 0, Qt::AlignLeft );
    }
    else if( 2 == type )
    {
        m_hlayout2->addWidget( m_ptSpinValue, 0, Qt::AlignLeft );
    }
    else if( 3 == type )
    {
        m_hlayout2->addWidget( m_ptDSpinValue, 0, Qt::AlignLeft );
    }

    m_vlayout->addLayout( m_hlayout2 );

    int  width = 2*m_hlayout2->margin() + 3*m_ptBtnValDec->minimumSize().width() + 2*m_hlayout2->spacing() + (m_depth + 1)*( m_ptBtnValDec->minimumSize().width() + m_hlayout2->spacing() );
    int  height = 2*m_hlayout2->margin() + m_ptBtnValDec->minimumSizeHint().height() + m_vlayout->spacing();

    widget_stretch( width, height, false );

    m_nlayout2height = height;

    m_second_row_exist = true;
}

void  TParam::setParamValueChange( unsigned  type )
{
    if( m_second_row_type != type )
    {
        m_second_row_type = type;

        m_ptLineValue->hide();
        m_ptSpinValue->hide();
        m_ptDSpinValue->hide();

        m_hlayout2->removeWidget( m_ptLineValue );
        m_hlayout2->removeWidget( m_ptSpinValue );
        m_hlayout2->removeWidget( m_ptDSpinValue );

        if( 1 == type )
        {
            m_hlayout2->addWidget( m_ptLineValue, 0, Qt::AlignLeft );
            m_ptLineValue->show();
        }
        else if( 2 == type )
        {
            m_hlayout2->addWidget( m_ptSpinValue, 0, Qt::AlignLeft );
            m_ptSpinValue->show();
        }
        else if( 3 == type )
        {
            m_hlayout2->addWidget( m_ptDSpinValue, 0, Qt::AlignLeft );
            m_ptDSpinValue->show();
        }
    }
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

    // уничтожаем layout
    m_hlayout2->deleteLater();

    widget_shrink( 0, m_nlayout2height );

    m_second_row_exist = false;
}

void  TParam::setParamValueMin( int  min ) noexcept
{
    if( m_second_row_exist )
    {
        m_ptSpinValue->setMinimum( min );
    }
}

void  TParam::setParamValueMax( int  max ) noexcept
{
    if( m_second_row_exist )
    {
        m_ptSpinValue->setMaximum( max );
    }
}

void  TParam::setParamValueDMin( double  min ) noexcept
{
    if( m_second_row_exist )
    {
        m_ptDSpinValue->setMinimum( min );
    }
}

void  TParam::setParamValueDMax( double  max ) noexcept
{
    if( m_second_row_exist )
    {
        m_ptDSpinValue->setMaximum( max );
    }
}

//------------------------------------------------------------------------------

void  TParam::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;
}

void  TParam::widget_stretch( int width, int height, bool add_height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    //qDebug() << "stretch" << width << height;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_stretch( width, height, add_height );
}

void  TParam::widget_parent_stretch( int width, int height, bool add_height ) noexcept
{
    int  val = 0;

    if( Q_NULLPTR != m_pAncestor )
    {
        if( add_height )
        {
            val = m_pAncestor->m_vlayout->spacing();
        }

        m_pAncestor->widget_stretch( width, height + val, false );
    }
    else if( Q_NULLPTR != m_pMentor )
    {
        if( add_height )
        {
            val = m_pMentor->m_vlayout->spacing();
        }

        m_pMentor->widget_stretch( width, height + val, false );
    }
}

void  TParam::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    //qDebug() << "shrink" << width << height;

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_shrink( width, height );
}

void  TParam::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_shrink( width, height );
    }
    else if( Q_NULLPTR != m_pMentor )
    {
        m_pMentor->widget_shrink( width, height );
    }
}

int TParam::getParamWidth() noexcept
{
    return minimumSize().width();
}

int TParam::getParamHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------
