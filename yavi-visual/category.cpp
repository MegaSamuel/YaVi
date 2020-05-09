#include "func.h"
#include "goods.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategory::TCategory()
{
    m_zName.clear();

    m_depth = 0;

//    resetRow();
//    resetColumn();

//    m_grid = new QGridLayout;
//    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );
//    m_grid->SetMinimumSize(  );
    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
//    m_vlayout->setMargin( 0 );

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setMinimumWidth( 94 );
    m_ptLblName->setAlignment( Qt::AlignCenter );
    m_ptLblName->setFrameStyle( QFrame::Panel | QFrame::Raised );
//    m_grid->addWidget( m_ptLblName, m_row, 0, Qt::AlignLeft );
    hlayout->addWidget( m_ptLblName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
//    m_grid->addWidget( m_ptBtnInc, m_row, 1, Qt::AlignLeft );
    hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    m_vlayout->addLayout( hlayout, 0 );

    // перевод строки и колонки в grid для следующих добавлений
//    nextRow();
//    nextColumn();

    this->setLayout( m_vlayout );
}

TCategory::~TCategory()
{
    QLayoutItem *child;

    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }
}

//------------------------------------------------------------------------------

void  TCategory::onBtnInc()
{
    qDebug() << getCategoryName() << "inc button";
}

//------------------------------------------------------------------------------

void  TCategory::getCategories( const YAML::Node&  node, TParam  *a_pParam, int  depth )
{
//    unsigned  val;
    std::string  str;

    TCategories  *pCategories;
    pCategories = new TCategories( depth );
    m_vlayout->addWidget( pCategories, 0, Qt::AlignLeft | Qt::AlignTop );

    // добавляем Categories с список класса TParam
    a_pParam->m_apCategoriesList.append(pCategories);

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    pCategories->setCategoriesName( str );
//    qDebug() << "name" << QString::fromStdString(str);

    if( __yaml_IsSequence( node[ GoodsParametersSection ] ) )
    {
//        qDebug() << GoodsParametersSection << "is a sequence";

        for( auto& par : node[ GoodsParametersSection ] )
        {
            TParam  *pParam;
            pParam = new TParam( pCategories->getCategoriesDepth() );

            // добавляем Parameters в список класса TCategories
            pCategories->m_apParamList.append(pParam);

            getParameters( par, pParam, pCategories->getCategoriesDepth() );
        }
    }
}

void  TCategory::getParameters( const YAML::Node&  node, TParam *a_pParam, int  depth )
{
    unsigned  val;
    std::string  str;

//    m_depth = depth;

    m_vlayout->addWidget( a_pParam, 0, Qt::AlignLeft | Qt::AlignTop );
    m_apParamList.append( a_pParam );
//    nextRow();

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    a_pParam->setParamName( str );
//    qDebug() << "name" << QString::fromStdString(str);

    // тип
    if( __yaml_IsScalar( node[ GoodsTypeSection ] ) )
    {
        val = node[ GoodsTypeSection ].as<unsigned>();
        a_pParam->setParamType( val );
//        qDebug() << "type" << val;
    }

    //
    str = __yaml_GetString( node, GoodsPlaceholderSection );
    a_pParam->setParamPlaceholder( str );
//    qDebug() << "placeholder" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsNewSection );
    a_pParam->setParamNew( str );
//    qDebug() << "new" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsAfterSection );
    a_pParam->setParamAfter( str );
//    qDebug() << "after" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsBeforeSection );
    a_pParam->setParamBefore( str );
//    qDebug() << "before" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsUlinkSection );
    a_pParam->setParamUlink( str );
//    qDebug() << "ulink" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsUnameSection );
    a_pParam->setParamUname( str );
//    qDebug() << "umane" << QString::fromStdString(str);

    // мин
    if( __yaml_IsScalar( node[ GoodsMinSection ] ) )
    {
        val = node[ GoodsMinSection ].as<unsigned>();
        a_pParam->setParamMin( val );
//        qDebug() << "min" << val;
    }

    // макс
    if( __yaml_IsScalar( node[ GoodsMaxSection ] ) )
    {
        val = node[ GoodsMaxSection ].as<unsigned>();
        a_pParam->setParamMax( val );
//        qDebug() << "max" << val;
    }

    //
    str = __yaml_GetString( node, GoodsMultiSection );
    a_pParam->setParamMulti( str );
//    qDebug() << "multi" << QString::fromStdString(str);

    // значение
    str = __yaml_GetString( node, GoodsValuesSection );

    if( 0 != QString::fromStdString(str).length() )
    {
//        qDebug() << "values" << QString::fromStdString(str);

        if( __yaml_IsSequence( node[ GoodsCategoriesSection ] ) )
        {
//            qDebug() << GoodsCategoriesSection << "is a sequence";

            for( auto& cat : node[ GoodsCategoriesSection ] )
            {
                getCategories( cat, a_pParam, depth );
            }
        }
    }
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
/*
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
*/
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
