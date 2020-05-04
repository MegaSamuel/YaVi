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

#include "goods.h"

//------------------------------------------------------------------------------

// Устраняет необходимость использования <boost/algorithm/string.hpp>
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

// Устраняет необходимость использования <boost/algorithm/string/trim.hpp>

// Удаляет пробелы в начале строки
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

static const std::string __yaml_GetString( const YAML::Node&  node, const std::string&  name, const std::string  def = "" )
{
	if( __yaml_IsScalar( node[ name ] ) )
		return node[ name ].as<std::string>();
	return def;
}

//------------------------------------------------------------------------------

class TGoodsPrivate
{
    friend class TGoods;
private:
    TGoods 	*ins__;

    std::string 	m_zId;
    std::string 	m_zName;
    std::string 	m_zVersion;
    std::string 	m_zDescription;
    std::string 	m_zDesignation;
    unsigned		m_uNetwork;
    std::vector<std::string> 		m_vDocuments; 	// перечень имен документов
    std::vector<std::string> 		m_vSuffixes; 	// перечень суффиксов для добавления к имени при установке АРМа
    // перечень состояний системы (секция states)
    std::vector<std::string> 		m_vStates;
    // перечень векторов с их размерами
    std::vector<std::pair<std::string,unsigned>> 	m_vVectors;

    YAML::Node 		config__;
    bool 		m_bNoModel;

    inline 	void clear()
    {
        m_vStates.clear();
        m_vVectors.clear();

        m_zId.clear();
        m_zName.clear();
        m_zVersion.clear();
        m_zDescription.clear();
        m_zDesignation.clear();
        m_uNetwork = 0;
        m_vDocuments.clear();
        m_vSuffixes.clear();

        m_bNoModel = false;
    }

    inline TGoodsPrivate( TGoods  *ins ) :
        ins__( ins )
    {
        clear();
        (void)ins__;
    }
};

bool TGoods::parse_yaml( const YAML::Node&  config )
{
    priv__->config__ = config;

    // категория (category)
    if( __yaml_IsMap( config[ GoodsCategorySection ] ) )
    {
        qDebug() << GoodsCategorySection << "is a map";
    }

    // таблица (tables)
    if( __yaml_IsSequence( config[ GoodsTableSection ] ) )
    {
        qDebug() << GoodsTableSection << "is a sequence";

        for( auto tab : config[ GoodsTableSection ] )
        {
            if( __yaml_IsScalar( tab[ GoodsTableId ] ) )
            {
                std::string  id = tab[ GoodsTableId ].as<std::string>();
                qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id);
//                priv__->m_vVectors.emplace_back( id, s );

                if( __yaml_IsScalar( tab[GoodsTableName] ) )
                {
                    std::string  name = tab[GoodsTableName].as<std::string>();
                    qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "name" << QString::fromStdString(name) ;
                }

                if( __yaml_IsSequence( tab[GoodsTableColumn] ) )
                {
                    qDebug() << "column detected";
                    for( auto col : tab[ GoodsTableColumn ] )
                    {
                        if( __yaml_IsScalar( col[ GoodsTableName ] ) )
                        {
                            std::string  name = col[GoodsTableName].as<std::string>();
                            qDebug() << GoodsTableColumn << "name" << QString::fromStdString(name) ;

                            if( __yaml_IsScalar( col[ GoodsTableValue ] ) )
                            {
                                const std::string 	val = col[ GoodsTableValue ].as<std::string>();
                                qDebug() << GoodsTableColumn << GoodsTableValue << QString::fromStdString(val);
                                //                            split( priv__->m_vValues, val, '\n' );
                            }
                        }
                    }
                }

                if( __yaml_IsSequence( tab[GoodsTableRow] ) )
                {
                    qDebug() << "row detected";
                    for( auto row : tab[ GoodsTableRow ] )
                    {
                        if( __yaml_IsScalar( row[ GoodsTableName ] ) )
                        {
                            std::string  name = row[GoodsTableName].as<std::string>();
                            qDebug() << GoodsTableRow << "name" << QString::fromStdString(name) ;

                            if( __yaml_IsScalar( row[ GoodsTableValue ] ) )
                            {
                                const std::string 	val = row[ GoodsTableValue ].as<std::string>();
                                qDebug() << GoodsTableRow << GoodsTableValue << QString::fromStdString(val);
                                //                            split( priv__->m_vValues, val, '\n' );
                            }
                        }
                    }
                }

                if( __yaml_IsScalar( tab[GoodsTableLink] ) )
                {
                    std::string  link = tab[GoodsTableLink].as<std::string>();
                    qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "link" << QString::fromStdString(link) ;
                }
            }
        }
    }

    /*
    if( __yaml_IsMap( config[ ProtocolHeaderSectionName ] ) )
    {
	    priv__->m_zId = __yaml_GetString( config[ ProtocolHeaderSectionName ], "id" );
	    priv__->m_zName = __yaml_GetString( config[ ProtocolHeaderSectionName ], "name" );
	    priv__->m_zVersion = __yaml_GetString( config[ ProtocolHeaderSectionName ], "version" );
	    priv__->m_zDescription = __yaml_GetString( config[ ProtocolHeaderSectionName ], "description" );
	    priv__->m_zDesignation = __yaml_GetString( config[ ProtocolHeaderSectionName ], "designation" );

        if( __yaml_IsScalar( config[ ProtocolHeaderSectionName ][ "network" ] ) )
        {
            priv__->m_uNetwork = config[ ProtocolHeaderSectionName ][ "network" ].as<unsigned>();
        }

	    // documents list
	    if( __yaml_IsScalar( config[ ProtocolHeaderSectionName ][ "documents" ] ) )
	    {
		    const std::string 	val = config[ ProtocolHeaderSectionName ][ "documents" ].as<std::string>();
            split( priv__->m_vDocuments, val, '\n' );
	    }
	    // suffixes list
	    if( __yaml_IsScalar( config[ ProtocolHeaderSectionName ][ "suffixes" ] ) )
	    {
		    const std::string 	val = config[ ProtocolHeaderSectionName ][ "suffixes" ].as<std::string>();
            split( priv__->m_vSuffixes, val, '\n' );
	    }
    }	
*/
	return true;
}

TGoods::TGoods()
{
    priv__ = std::unique_ptr<TGoodsPrivate>( new TGoodsPrivate( this ) );
    clear();
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

void TGoods::clear() noexcept
{
    priv__->clear();
}

bool TGoods::empty() const noexcept
{
    return false;
//    return priv__->m_zId.empty();
}

//------------------------------------------------------------------------------
