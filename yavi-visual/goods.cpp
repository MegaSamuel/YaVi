#include <limits.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <sys/stat.h>

#ifdef QT_CORE_LIB
#include <QtCore>
#include <QString>
#include <QDebug>
#endif

#include "func.h"
#include "goods.h"

//------------------------------------------------------------------------------

// Устраняет необходимость использования <boost/algorithm/string.hpp>
/*
template<typename SequenceSequenceT, typename CharT>
SequenceSequenceT&  split( SequenceSequenceT& result, const std::string& input, CharT delim )
{
    std::stringstream ss( input );
    std::string item;
    while( std::getline( ss, item, delim ) ) {
        result.push_back( item );
    }
    return result;
}
*/
// Устраняет необходимость использования <boost/algorithm/string/trim.hpp>

// Удаляет пробелы в начале строки
/*
static inline void ltrim( std::string& s ) {
    s.erase( s.begin(), std::find_if( s.begin(), s.end(), []( int ch ) {
        return !std::isspace( ch );
    } ) );
}

// Удаляет пробелы в конце строки
static inline void rtrim( std::string& s ) {
    s.erase( std::find_if( s.rbegin(), s.rend(), []( int ch ) {
        return !std::isspace( ch );
    } ).base(), s.end() );
}

// Удаляет пробелы в начале и в конце строки
static inline void trim( std::string& s ) {
    ltrim( s );
    rtrim( s );
}
*/
/*
static bool __yaml_IsScalar( const YAML::Node&  node )
{
	return ( node.IsDefined() ? ( node.IsScalar() ? true : false ) : false );
}

static bool __yaml_IsSequence( const YAML::Node&  node )
{
	return ( node.IsDefined() ? ( node.IsSequence() ? true : false ) : false );
}

static bool __yaml_IsMap( const YAML::Node&  node )
{
	return ( node.IsDefined() ? ( node.IsMap() ? true : false ) : false );
}

static const std::string __yaml_GetString( const YAML::Node&  node, const std::string&  name, const std::string  def = GoodsDefName )
{
	if( __yaml_IsScalar( node[ name ] ) )
		return node[ name ].as<std::string>();
	return def;
}
*/
//------------------------------------------------------------------------------

class TGoodsPrivate
{
    friend class TGoods;
private:
    TGoods 	*ins__;

//    std::vector<std::string> 		m_vDocuments; 	// перечень имен документов
//    std::vector<std::string> 		m_vSuffixes; 	// перечень суффиксов для добавления к имени при установке АРМа
    // перечень состояний системы (секция states)
//    std::vector<std::string> 		m_vStates;
    // перечень векторов с их размерами
//    std::vector<std::pair<std::string,unsigned>> 	m_vVectors;

    bool  m_bEmpty;

    YAML::Node 		config__;

    QList<TTable*>  m_apTableList;
    QList<TCategory*>  m_apCategoryList;

    QStringList     m_vValues;
//    std::vector<std::string>  m_vValues;

    inline 	void clear()
    {
        m_bEmpty = true;

        for( auto& it : m_apTableList )
        {
            it->~TTable();
        }

        for( auto& it : m_apCategoryList )
        {
            it->~TCategory();
        }

        m_apTableList.clear();

        m_apCategoryList.clear();

        m_vValues.clear();

//        m_vStates.clear();
//        m_vVectors.clear();

//        m_vDocuments.clear();
//        m_vSuffixes.clear();
    }

    inline TGoodsPrivate( TGoods  *ins ) :
        ins__( ins )
    {
        clear();
        (void)ins__;
    }
};

//------------------------------------------------------------------------------

bool TGoods::parse_yaml( const YAML::Node&  config )
{
    priv__->config__ = config;

    // категория (category)
    if( __yaml_IsSequence( config[ GoodsCategorySection ] ) )
    {
        priv__->m_bEmpty = false;

        qDebug() << GoodsCategorySection << "is a sequence";

        for( auto& cat : config[ GoodsCategorySection ] )
        {
            TCategory  *pCategory;
            pCategory = new TCategory();
            m_vlayout->addWidget( pCategory );
            priv__->m_apCategoryList.append(pCategory);

            // ищем имя
            std::string cat_name = __yaml_GetString( cat, GoodsCategoryName );
            qDebug() << GoodsCategorySection << "name is" << QString::fromStdString(cat_name);
            pCategory->setCategoryName( cat_name );

            if( __yaml_IsSequence( cat[ GoodsParametersSection ] ) )
            {
                for( auto& par : cat[ GoodsParametersSection ] )
                {
//                    get_parameters(par);

                    pCategory->getParameters( par );
                }
            }

            fix_widget_size( pCategory->getCategoryWidth(), pCategory->getCategoryHeight() );
        }
    }

    // таблица (tables)
    if( __yaml_IsSequence( config[ GoodsTableSection ] ) )
    {
        priv__->m_bEmpty = false;

//        qDebug() << GoodsTableSection << "is a sequence";

        for( auto& tab : config[ GoodsTableSection ] )
        {
            bool  table_fill = false; // признак что таблица не заполнена

            TTable  *pTable;
            pTable = new TTable();
            m_vlayout->addWidget( pTable );
            priv__->m_apTableList.append(pTable);

            // ищем секцию id
            std::string  id = __yaml_GetString( tab, GoodsTableId );
//            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id);
            pTable->setTableId( id );

            // ищем имя секции id
            std::string  id_name = __yaml_GetString( tab, GoodsTableName );
//            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "name" << QString::fromStdString(id_name) ;
            pTable->setTableName( id_name );

            // ищем столбцы
            if( !table_fill && __yaml_IsSequence( tab[GoodsTableColumn] ) )
            {
                for( auto& col : tab[ GoodsTableColumn ] )
                {
                    QStringList  col_list;
                    col_list.clear();

                    // имя
                    std::string  col_name = __yaml_GetString( col, GoodsTableName );
//                    qDebug() << GoodsTableColumn << "name" << QString::fromStdString(col_name) ;
                    col_list.append( QString::fromStdString(col_name) );

                    // значение
                    const std::string  col_val = __yaml_GetString( col, GoodsTableValue );
//                    qDebug() << GoodsTableColumn << GoodsTableValue << QString::fromStdString(col_val);
                    priv__->m_vValues.clear();
                    priv__->m_vValues = QString::fromStdString(col_val).split( '\n', QString::SkipEmptyParts );

                    for( auto& it : priv__->m_vValues )
                    {
                        col_list.append( it );
                    }

                    pTable->setTableColumn( col_list );
                }

                pTable->nextRow();

                table_fill = true;
            }

            // ищем строки
            if( !table_fill && __yaml_IsSequence( tab[GoodsTableRow] ) )
            {
                for( auto& row : tab[ GoodsTableRow ] )
                {
                    QStringList  row_list;
                    row_list.clear();

                    // имя
                    std::string  row_name = __yaml_GetString( row, GoodsTableName );
//                    qDebug() << GoodsTableRow << "name" << QString::fromStdString(row_name) ;
                    row_list.append( QString::fromStdString(row_name) );

                    // значение
                    const std::string  row_val = __yaml_GetString( row, GoodsTableValue );
//                    qDebug() << GoodsTableRow << GoodsTableValue << QString::fromStdString(row_val);
                    priv__->m_vValues.clear();
                    priv__->m_vValues = QString::fromStdString(row_val).split( '\n', QString::SkipEmptyParts );

                    for( auto& it : priv__->m_vValues )
                    {
                        row_list.append( it );
                    }

                    pTable->setTableRow( row_list );
                }

                table_fill = true;
            }

            // ищем ссылку
            if( !table_fill )
            {
                std::string  link = __yaml_GetString( tab, GoodsTableLink );
//                qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "link" << QString::fromStdString(link) ;
                pTable->setTableLink( link );
            }

            fix_widget_size( pTable->getTableWidth(), pTable->getTableHeight() );
        }
    }

//    qDebug() << "TableList size" << priv__->m_apTableList.size();

	return true;
}

//------------------------------------------------------------------------------

void  TGoods::get_categories( const YAML::Node&  node )
{
//    unsigned  val;
    std::string  str;

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    qDebug() << "name" << QString::fromStdString(str);

    if( __yaml_IsSequence( node[ GoodsParametersSection ] ) )
    {
//        qDebug() << GoodsParametersSection << "is a sequence";

        for( auto& par : node[ GoodsParametersSection ] )
        {
            get_parameters( par );
        }
    }
}

void  TGoods::get_parameters( const YAML::Node&  node )
{
    unsigned  val;
    std::string  str;

    // имя
    str = __yaml_GetString( node, GoodsNameSection );
    qDebug() << "name" << QString::fromStdString(str);

    // тип
    if( __yaml_IsScalar( node[ GoodsTypeSection ] ) )
    {
        val = node[ GoodsTypeSection ].as<unsigned>();
//        qDebug() << "type" << val;
    }

    //
    str = __yaml_GetString( node, GoodsPlaceholderSection );
//    qDebug() << "placeholder" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsNewSection );
//    qDebug() << "new" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsAfterSection );
//    qDebug() << "after" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsBeforeSection );
//    qDebug() << "before" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsUlinkSection );
//    qDebug() << "ulink" << QString::fromStdString(str);

    //
    str = __yaml_GetString( node, GoodsUnameSection );
//    qDebug() << "umane" << QString::fromStdString(str);

    // мин
    if( __yaml_IsScalar( node[ GoodsMinSection ] ) )
    {
        val = node[ GoodsMinSection ].as<unsigned>();
//        qDebug() << "min" << val;
    }

    // макс
    if( __yaml_IsScalar( node[ GoodsMaxSection ] ) )
    {
        val = node[ GoodsMaxSection ].as<unsigned>();
//        qDebug() << "max" << val;
    }

    //
    str = __yaml_GetString( node, GoodsMultiSection );
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
                get_categories( cat );
            }
        }
    }
}

//------------------------------------------------------------------------------

TGoods::TGoods()
{
    priv__ = std::unique_ptr<TGoodsPrivate>( new TGoodsPrivate( this ) );
    clear();

    // в вертикальный layout будем складывать элементы из ямла
    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // ставим начальный размер себя
    m_w = 100;
    m_h = 100;
    fix_widget_size( 0, 0 );

    this->setLayout( m_vlayout );
}

TGoods::TGoods( const YAML::Node&  config )
{
    priv__ = std::unique_ptr<TGoodsPrivate>( new TGoodsPrivate( this ) );
    clear();
    if( ! parse_yaml( config ) )
		clear();
}

TGoods::~TGoods()
{
    clear();
}

//------------------------------------------------------------------------------

void  TGoods::clear() noexcept
{
    priv__->clear();
}

bool  TGoods::empty() const noexcept
{
    return priv__->m_bEmpty;
}

int  TGoods::get_table_size() noexcept
{
    return priv__->m_apTableList.size();
}

void  TGoods::fix_widget_size( int w, int h ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( w > m_w )
        m_w = w;

    // высоту увеличиваем на каждый элемент
    m_h += h;

    // ставим размер самого себя
    this->setMinimumWidth( m_w );
    this->setMinimumHeight( m_h );
}

//------------------------------------------------------------------------------
