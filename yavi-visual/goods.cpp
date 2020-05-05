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

//------------------------------------------------------------------------------

TTable::TTable()
{
    m_hlayout = new QHBoxLayout;

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-", this );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    m_hlayout->addWidget( m_ptBtnDec, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setFrameStyle( QFrame::NoFrame );
    m_hlayout->addWidget( m_ptLblName, 0, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_hlayout->addWidget( m_ptBtnInc, 0, Qt::AlignLeft );

    // пружинка
    m_hlayout->addStretch( 0 );

    this->setLayout( m_hlayout );

    qDebug() << "TTable construct";
}

TTable::~TTable()
{
    while( QLayoutItem* item = m_hlayout->takeAt(0) )
    {
        delete item->widget();
        delete item;
    }
}

void  TTable::onBtnDec()
{
    qDebug() << "Dec button";
}

void  TTable::onBtnInc()
{
    qDebug() << "Inc button";
}

void  TTable::setTableName( const std::string&  name )
{
    m_ptLblName->setText( QString::fromStdString(name) );
    qDebug() << "Set table name" << QString::fromStdString(name);
}

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

    inline 	void clear()
    {
        m_bEmpty = true;

        for( auto& it : m_apTableList )
        {
            it->~TTable();
        }

        m_apTableList.clear();

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
    if( __yaml_IsMap( config[ GoodsCategorySection ] ) )
    {
        priv__->m_bEmpty = false;
        qDebug() << GoodsCategorySection << "is a map";
    }

    // таблица (tables)
    if( __yaml_IsSequence( config[ GoodsTableSection ] ) )
    {
        priv__->m_bEmpty = false;
        qDebug() << GoodsTableSection << "is a sequence";

        for( auto& tab : config[ GoodsTableSection ] )
        {
            TTable  *pTable;
            pTable = new TTable();
            m_vlayout->addWidget( pTable );
            priv__->m_apTableList.append(pTable);

            // ищем секцию id
            std::string  id = __yaml_GetString( tab, GoodsTableId );
            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id);
//            priv__->m_vVectors.emplace_back( id, s );

            // ищем имя секции id
            std::string  id_name = __yaml_GetString( tab, GoodsTableName );
            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "name" << QString::fromStdString(id_name) ;
            pTable->setTableName( id_name );

            // ищем столбцы
            if( __yaml_IsSequence( tab[GoodsTableColumn] ) )
            {
                for( auto& col : tab[ GoodsTableColumn ] )
                {
                    // имя
                    std::string  col_name = __yaml_GetString( col, GoodsTableName );
                    qDebug() << GoodsTableColumn << "name" << QString::fromStdString(col_name) ;

                    // значение
                    const std::string  col_val = __yaml_GetString( col, GoodsTableValue );
                    qDebug() << GoodsTableColumn << GoodsTableValue << QString::fromStdString(col_val);
                    //                            split( priv__->m_vValues, val, '\n' );
                }
            }

            // ищем строки
            if( __yaml_IsSequence( tab[GoodsTableRow] ) )
            {
                for( auto& row : tab[ GoodsTableRow ] )
                {
                    // имя
                    std::string  row_name = __yaml_GetString( row, GoodsTableName );
                    qDebug() << GoodsTableRow << "name" << QString::fromStdString(row_name) ;

                    // значение
                    const std::string  row_val = __yaml_GetString( row, GoodsTableValue );
                    qDebug() << GoodsTableRow << GoodsTableValue << QString::fromStdString(row_val);
                    //                            split( priv__->m_vValues, val, '\n' );
                }
            }

            // ищем ссылку
            std::string  link = __yaml_GetString( tab, GoodsTableLink );
            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "link" << QString::fromStdString(link) ;
        }
    }

    qDebug() << "TableList size" << priv__->m_apTableList.size();

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

    m_vlayout = new QVBoxLayout;
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

void TGoods::clear() noexcept
{
    priv__->clear();
}

bool TGoods::empty() const noexcept
{
    return priv__->m_bEmpty;
}

int TGoods::get_table_size() noexcept
{
    return priv__->m_apTableList.size();
}

//------------------------------------------------------------------------------
