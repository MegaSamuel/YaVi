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

bool TGoods::parse_yaml( const YAML::Node&  config )
{
    // категория (category)
    if( __yaml_IsSequence( config[ GoodsCategorySection ] ) )
    {
        // файл не пуст
        m_bEmpty = false;

        TCategory  *pCategory;

        // по заданию в ини-файле только одна Category, но на всякий случай ищем все
        for( int j = 0; j < static_cast<int>(config[ GoodsCategorySection ].size()); j++ )
        {
            pCategory = new TCategory( this );
            pCategory->setNode( config[ GoodsCategorySection ][j] );
            pCategory->setNodeParent( config[ GoodsCategorySection ] );
            pCategory->setNodeIndex( j );

            m_vlayout->addWidget( pCategory );
            m_apCategoryList.append(pCategory);

            // ищем имя
            std::string cat_name = __yaml_GetString( config[ GoodsCategorySection ][j], GoodsCategoryName );
            pCategory->setCategoryName( cat_name );

            if( __yaml_IsSequence( config[ GoodsCategorySection ][j][ GoodsParametersSection ] ) )
            {
                TParam  *pParam;

                for( int i = 0; i < static_cast<int>(config[ GoodsCategorySection ][j][ GoodsParametersSection ].size()); i++ )
                {
                    pParam = new TParam( pCategory, Q_NULLPTR, 0 );
                    pParam->setNode( config[ GoodsCategorySection ][j][ GoodsParametersSection ][i] );
                    pParam->setNodeParent( config[ GoodsCategorySection ][j][ GoodsParametersSection ] );
                    pParam->setNodeIndex( i );

                    // добавляемся тут, внутри нельзя, т.к. getParameters вызывается разными классами
                    pCategory->m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
                    pCategory->m_apParamList.append( pParam );

                    pCategory->getParameters( config[ GoodsCategorySection ][j][ GoodsParametersSection ][i], pParam, pParam->getParamDepth() );
                }
            }
        }
    }

    // таблица (tables)
    if( __yaml_IsSequence( config[ GoodsTableSection ] ) )
    {
        // файл не пуст
        m_bEmpty = false;

        TTable  *pTable;

        for( int j = 0; j < static_cast<int>(config[ GoodsTableSection ].size()); j++ )
        {
            bool  table_fill = false; // признак что таблица не заполнена

            pTable = new TTable( this );
            pTable->setNode( config[ GoodsTableSection ][j] );
            pTable->setNodeParent( config[ GoodsTableSection ] );
            pTable->setNodeIndex( j );

            m_vlayout->addWidget( pTable );
            m_apTableList.append(pTable);

            // ищем секцию id
            std::string  id = __yaml_GetString( config[ GoodsTableSection ][j], GoodsTableId );
            pTable->setTableId( id );

            // ищем имя секции id
            std::string  id_name = __yaml_GetString( config[ GoodsTableSection ][j], GoodsTableName );
            pTable->setTableName( id_name );

            // ищем столбцы
            if( !table_fill && __yaml_IsSequence( config[ GoodsTableSection ][j][GoodsTableColumn] ) )
            {
                for( auto& col : config[ GoodsTableSection ][j][ GoodsTableColumn ] )
                {
                    QStringList  col_list;
                    col_list.clear();

                    // имя
                    std::string  col_name = __yaml_GetString( col, GoodsTableName );
                    col_list.append( QString::fromStdString(col_name) );

                    // значение
                    QStringList  vValues;
                    const std::string  col_val = __yaml_GetString( col, GoodsTableValue );
                    vValues.clear();
                    vValues = QString::fromStdString(col_val).split( '\n', QString::SkipEmptyParts );

                    for( auto& it : vValues )
                    {
                        col_list.append( it );
                    }

                    pTable->setTableColumn( col_list );
                }

                pTable->nextRow();

                pTable->setTableType( TTable::keTypeRow );

                table_fill = true;
            }

            // ищем строки
            if( !table_fill && __yaml_IsSequence( config[ GoodsTableSection ][j][GoodsTableRow] ) )
            {
                for( auto& row : config[ GoodsTableSection ][j][ GoodsTableRow ] )
                {
                    QStringList  row_list;
                    row_list.clear();

                    // имя
                    std::string  row_name = __yaml_GetString( row, GoodsTableName );
                    row_list.append( QString::fromStdString(row_name) );

                    // значение
                    QStringList  vValues;
                    const std::string  row_val = __yaml_GetString( row, GoodsTableValue );
                    vValues.clear();
                    vValues = QString::fromStdString(row_val).split( '\n', QString::SkipEmptyParts );

                    for( auto& it : vValues )
                    {
                        row_list.append( it );
                    }

                    pTable->setTableRow( row_list );
                }

                pTable->setTableType( TTable::keTypeColumn );

                table_fill = true;
            }

            // ищем ссылку
            if( !table_fill )
            {
                std::string  link = __yaml_GetString( config[ GoodsTableSection ][j], GoodsTableLink );
                pTable->setTableLink( link );

                pTable->setTableType( TTable::keTypeLink );
            }
        }
    }

	return true;
}

//------------------------------------------------------------------------------

TGoods::TGoods()
{
    clear();

    // в вертикальный layout будем складывать элементы из ямла
    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    setLayout( m_vlayout );

    widget_size_reset();

    widget_stretch( m_vlayout->minimumSize().width(), m_vlayout->minimumSize().height() );
}

TGoods::~TGoods()
{

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

    widget_size_reset();

    widget_stretch( 0, 0 );
}

void  TGoods::clear() noexcept
{
    m_bEmpty = true;

    m_apTableList.clear();
    m_apCategoryList.clear();
}

bool  TGoods::empty() const noexcept
{
    return m_bEmpty;
}

void  TGoods::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;
}

void  TGoods::widget_stretch( int width, int height ) noexcept
{
    width += 2*m_vlayout->margin();

    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    //qDebug() << "wgt stretch" << width << height << "size" << m_width << m_height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );
}

void  TGoods::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    //qDebug() << "wgt shrink" << height << "height" << m_height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );
}

int TGoods::getWidgetWidth() noexcept
{
    return minimumSize().width();
}

int TGoods::getWidgetHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------
