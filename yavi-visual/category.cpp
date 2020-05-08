#include "func.h"
#include "goods.h"
#include "category.h"

//------------------------------------------------------------------------------

TCategory::TCategory()
{
    m_zName.clear();

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );
//    m_grid->SetMinimumSize(  );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setMinimumWidth( 94 );
    m_ptLblName->setAlignment( Qt::AlignCenter );
    m_ptLblName->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblName, m_row, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_grid->addWidget( m_ptBtnInc, m_row, 1, Qt::AlignLeft );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    this->setLayout( m_grid );
}

TCategory::~TCategory()
{
    QLayoutItem *child;

    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }
}

//------------------------------------------------------------------------------

void  TCategory::onBtnInc()
{
    qDebug() << m_zName << "inc button";
}

//------------------------------------------------------------------------------

void  TCategory::getCategories( const YAML::Node&  node )
{
//    unsigned  val;
    std::string  str;

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    qDebug() << "name" << QString::fromStdString(str);

    if( __yaml_IsSequence( node[ GoodsParametersSection ] ) )
    {
        qDebug() << GoodsParametersSection << "is a sequence";

//        for( auto& par : node[ GoodsParametersSection ] )
//        {
//            get_parameters( par );
//        }
    }
}

void  TCategory::getParameters( const YAML::Node&  node )
{
    unsigned  val;
    std::string  str;

    TParam  *pParam;
    pParam = new TParam();
    m_grid->addWidget( pParam, m_row, 1, Qt::AlignLeft );
    m_apParamList.append(pParam);
    nextRow();

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    pParam->setParamName( str );
    qDebug() << "name" << QString::fromStdString(str);

    // тип
    if( __yaml_IsScalar( node[ GoodsTypeSection ] ) )
    {
        val = node[ GoodsTypeSection ].as<unsigned>();
        pParam->setParamType( val );
//        qDebug() << "type" << val;
    }

    //
    str = __yaml_GetString( node, GoodsPlaceholderSection );
    pParam->setParamPlaceholder( str );
//    qDebug() << "placeholder" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsNewSection );
    pParam->setParamNew( str );
//    qDebug() << "new" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsAfterSection );
    pParam->setParamAfter( str );
//    qDebug() << "after" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsBeforeSection );
    pParam->setParamBefore( str );
//    qDebug() << "before" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsUlinkSection );
    pParam->setParamUlink( str );
//    qDebug() << "ulink" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsUnameSection );
    pParam->setParamUname( str );
//    qDebug() << "umane" << QString::fromStdString(str);

    // мин
    if( __yaml_IsScalar( node[ GoodsMinSection ] ) )
    {
        val = node[ GoodsMinSection ].as<unsigned>();
        pParam->setParamMin( val );
//        qDebug() << "min" << val;
    }

    // макс
    if( __yaml_IsScalar( node[ GoodsMaxSection ] ) )
    {
        val = node[ GoodsMaxSection ].as<unsigned>();
        pParam->setParamMax( val );
//        qDebug() << "max" << val;
    }

    //
    str = __yaml_GetString( node, GoodsMultiSection );
    pParam->setParamMulti( str );
//    qDebug() << "multi" << QString::fromStdString(str);

    // значение
    str = __yaml_GetString( node, GoodsValuesSection );

    if( 0 != QString::fromStdString(str).length() )
    {
        qDebug() << "values" << QString::fromStdString(str);

        if( __yaml_IsSequence( node[ GoodsCategoriesSection ] ) )
        {
//            qDebug() << GoodsCategoriesSection << "is a sequence";

            for( auto& cat : node[ GoodsCategoriesSection ] )
            {
                getCategories( cat );
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

int TCategory::getCategoryWidth()
{
    QSize size = m_grid->minimumSize();

    return size.width();
}

int TCategory::getCategoryHeight()
{
    QSize size = m_grid->minimumSize();

    return size.height();
}

//------------------------------------------------------------------------------
