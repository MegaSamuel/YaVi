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

//------------------------------------------------------------------------------

class TGoodsPrivate
{
    friend class TGoods;
private:
    TGoods 	*ins__;

    bool  m_bEmpty;

    YAML::Node 		config__;

    QStringList     m_vValues;

    inline 	void clear()
    {
        m_bEmpty = true;

        m_vValues.clear();
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
        // файл не пуст
        priv__->m_bEmpty = false;

        // по заданию в ини-файле только одна Category, но на всякий случай ищем все
        for( auto& cat : config[ GoodsCategorySection ] )
        {
            TCategory  *pCategory;
            pCategory = new TCategory( this );
            pCategory->setNode( cat );
            m_vlayout->addWidget( pCategory );
            m_apCategoryList.append(pCategory);

            // ищем имя
            std::string cat_name = __yaml_GetString( cat, GoodsCategoryName );
            pCategory->setCategoryName( cat_name );

            if( __yaml_IsSequence( cat[ GoodsParametersSection ] ) )
            {
                for( auto& par : cat[ GoodsParametersSection ] )
                {
                    TParam  *pParam;
                    pParam = new TParam( pCategory, Q_NULLPTR, 0 );
                    pParam->setNode( par );

                    pCategory->getParameters( par, pParam, 0 );
                }
            }

            // подгоняем размер виджета под содержимое для корректной работы скролла
            widget_stretch( pCategory->getCategoryWidth(), pCategory->getCategoryHeight() );
        }
    }

    // таблица (tables)
    if( __yaml_IsSequence( config[ GoodsTableSection ] ) )
    {
        // файл не пуст
        priv__->m_bEmpty = false;

        for( auto& tab : config[ GoodsTableSection ] )
        {
            bool  table_fill = false; // признак что таблица не заполнена

            TTable  *pTable;
            pTable = new TTable( this );
            m_vlayout->addWidget( pTable );
            m_apTableList.append(pTable);

            // ищем секцию id
            std::string  id = __yaml_GetString( tab, GoodsTableId );
            pTable->setTableId( id );

            // ищем имя секции id
            std::string  id_name = __yaml_GetString( tab, GoodsTableName );
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
                    col_list.append( QString::fromStdString(col_name) );

                    // значение
                    const std::string  col_val = __yaml_GetString( col, GoodsTableValue );
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
                    row_list.append( QString::fromStdString(row_name) );

                    // значение
                    const std::string  row_val = __yaml_GetString( row, GoodsTableValue );
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
                pTable->setTableLink( link );
            }

            // подгоняем размер виджета под содержимое для корректной работы скролла
            widget_stretch( pTable->getTableWidth(), pTable->getTableHeight() );
        }
    }

	return true;
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
    widget_size_reset();

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

void  TGoods::GoodsDelete()
{
    // категории
    for( auto& it : m_apCategoryList )
    {
        // очищаем
        it->CategoryDelete();

        // уничтожаем
        it->~TCategory();
    }

    // таблицы
    for( auto& it : m_apTableList )
    {
        // очищаем
        it->TableDelete();

        // уничтожаем
        it->~TTable();
    }

    clear();

    // ставим начальный размер себя
    widget_size_reset();
}

void  TGoods::clear() noexcept
{
    m_apTableList.clear();
    m_apCategoryList.clear();

    priv__->clear();
}

bool  TGoods::empty() const noexcept
{
    return priv__->m_bEmpty;
}

void  TGoods::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;

    // ставим размер самого себя
    this->setMinimumWidth( m_width );
    this->setMinimumHeight( m_height );
}

void  TGoods::widget_stretch( int width, int height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    // ставим размер самого себя
    this->setMinimumWidth( m_width );
    this->setMinimumHeight( m_height );
}

void  TGoods::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    this->setMinimumHeight( m_height );
}

//------------------------------------------------------------------------------
