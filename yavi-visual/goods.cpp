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
    m_zId.clear();
    m_zName.clear();
    m_zLink.clear();

    resetRow();
    resetColumn();

    m_grid = new QGridLayout;

    // кнопка минус
    m_ptBtnDec = new QPushButton( "-", this );
    connect( m_ptBtnDec, SIGNAL(clicked()), this, SLOT(onBtnDec()) );
    m_grid->addWidget( m_ptBtnDec, m_row, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblName = new QLabel( this, Q_NULLPTR );
    m_ptLblName->setText( "m_ptLblName" );
    m_ptLblName->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblName, m_row, 1, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+", this );
    connect( m_ptBtnInc, SIGNAL(clicked()), this, SLOT(onBtnInc()) );
    m_grid->addWidget( m_ptBtnInc, m_row, 2, Qt::AlignLeft );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    this->setLayout( m_grid );
}

TTable::~TTable()
{
    QLayoutItem *child;

    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }
}

void  TTable::onBtnDec()
{
    qDebug() << "Dec button" << getTableId() << getTableName();
}

void  TTable::onBtnInc()
{
    qDebug() << "Inc button" << getTableId() << getTableName();
}

void  TTable::setTableId( const std::string&  name )
{
    m_zId = QString::fromStdString(name);
}

const QString TTable::getTableId()
{
    return m_zId;
}

void  TTable::setTableName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);
    m_ptLblName->setText( m_zName );
}

const QString TTable::getTableName()
{
    return m_zName;
}

void  TTable::setTableLink( const std::string&  name )
{
    m_zLink = QString::fromStdString(name);

    m_ptLblLink = new QLabel( this, Q_NULLPTR );
    m_ptLblLink->setText( m_zLink );
    m_ptLblLink->setMinimumSize(95,25);
    m_ptLblLink->setFrameStyle( QFrame::Panel | QFrame::Raised );
    m_grid->addWidget( m_ptLblLink, m_row, 1, 1, -1, Qt::AlignLeft );

    nextRow();
}

const QString TTable::getTableLink()
{
    return m_zLink;
}

void TTable::setTableRow( QStringList& list )
{
    int column = 0;

    for( auto& it : list )
    {
        QLabel  *label = new QLabel( this, Q_NULLPTR );
        label->setText( it );
        label->setMinimumSize(95,25);
        label->setFrameStyle( QFrame::Panel | QFrame::Raised );
        m_grid->addWidget( label, m_row, m_column + column, Qt::AlignLeft );

        column += 1;
    }

    nextRow();
}

void TTable::setTableColumn( QStringList& list )
{
    int row = 0;

    for( auto& it : list )
    {
        QLabel  *label = new QLabel( this, Q_NULLPTR );
        label->setText( it );
        label->setMinimumSize(95,25);
        label->setFrameStyle( QFrame::Panel | QFrame::Raised );
        m_grid->addWidget( label, m_row + row, m_column, Qt::AlignLeft );

        row += 1;
    }

    nextColumn();
}

void TTable::resetRow()
{
    m_row = 0;
}

void TTable::resetColumn()
{
    m_column = 0;
}


void TTable::nextRow()
{
    m_row += 1;
}

void TTable::nextColumn()
{
    m_column += 1;
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

    QStringList     m_vValues;
//    std::vector<std::string>  m_vValues;

    inline 	void clear()
    {
        m_bEmpty = true;

        for( auto& it : m_apTableList )
        {
            it->~TTable();
        }

        m_apTableList.clear();

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
            bool  table_fill = false; // признак что таблица не заполнена

            TTable  *pTable;
            pTable = new TTable();
            m_vlayout->addWidget( pTable );
            priv__->m_apTableList.append(pTable);

            // ищем секцию id
            std::string  id = __yaml_GetString( tab, GoodsTableId );
//            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id);
            pTable->setTableId( id );
//            priv__->m_vVectors.emplace_back( id, s );

            // ищем имя секции id
            std::string  id_name = __yaml_GetString( tab, GoodsTableName );
//            qDebug() << GoodsTableSection <<  GoodsTableId << "is a" << QString::fromStdString(id) << "name" << QString::fromStdString(id_name) ;
            pTable->setTableName( id_name );

            // ищем столбцы
            if( !table_fill && __yaml_IsSequence( tab[GoodsTableColumn] ) )
            {
                for( auto& col : tab[ GoodsTableColumn ] )
                {
                    QString tmp;
                    QStringList  col_list;
                    col_list.clear();

                    // имя
                    std::string  col_name = __yaml_GetString( col, GoodsTableName );
//                    qDebug() << GoodsTableColumn << "name" << QString::fromStdString(col_name) ;
                    col_list.append( QString::fromStdString(col_name) );

                    // значение
                    const std::string  col_val = __yaml_GetString( col, GoodsTableValue );
//                    qDebug() << GoodsTableColumn << GoodsTableValue << QString::fromStdString(col_val);
                    tmp = QString::fromStdString(col_val);
                    tmp.remove( QRegExp("\\\\\\|-") );
                    priv__->m_vValues.clear();
                    priv__->m_vValues = tmp.split( ' ', QString::SkipEmptyParts );

//                    qDebug() << priv__->m_vValues.size();

                    //split( priv__->m_vValues, col_val, '\n' );

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
                    QString tmp;
                    QStringList  row_list;
                    row_list.clear();

                    // имя
                    std::string  row_name = __yaml_GetString( row, GoodsTableName );
//                    qDebug() << GoodsTableRow << "name" << QString::fromStdString(row_name) ;
                    row_list.append( QString::fromStdString(row_name) );

                    // значение
                    const std::string  row_val = __yaml_GetString( row, GoodsTableValue );
//                    qDebug() << GoodsTableRow << GoodsTableValue << QString::fromStdString(row_val);
                    tmp = QString::fromStdString(row_val);
                    tmp.remove( QRegExp("\\\\\\|-") );
                    priv__->m_vValues.clear();
                    priv__->m_vValues = tmp.split( ' ', QString::SkipEmptyParts );
//                    split( priv__->m_vValues, row_val, '\n' );

//                    qDebug() << priv__->m_vValues.size();

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
        }
    }

//    qDebug() << "TableList size" << priv__->m_apTableList.size();

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

    QWidget *wgt = new QWidget;
    QScrollArea *scroll = new QScrollArea;
    QVBoxLayout *vlayout = new QVBoxLayout;

    wgt->resize(1000, 1000);
    wgt->setLayout(m_vlayout);

    scroll->setWidget(wgt);
//    scroll->setAlignment( Qt::AlignHCenter | Qt::AlignTop );
//    scroll->setMinimumWidth(500);
//    scroll->setMinimumHeight(500);
    scroll->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    scroll->setWidgetResizable(true);

    vlayout->addWidget( scroll );
    vlayout->setAlignment( scroll, Qt::AlignHCenter | Qt::AlignTop );

    this->setLayout( vlayout );
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
