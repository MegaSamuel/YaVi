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

TGoods::TGoods()
{
    clear();

    // в вертикальный layout будем складывать элементы из ямла
    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    setLayout( m_vlayout );

    widget_size_reset();

    widget_stretch( 2*m_vlayout->margin(), 2*m_vlayout->margin() );
}

TGoods::~TGoods()
{

}

void  TGoods::GoodsDelete()
{
    QLayoutItem *child;

    // категории
    for( auto& it : m_apCategoryList )
    {
        // очищаем
        it->CategoryDelete();

        // удаляем
        delete it;
    }

    // таблицы
    for( auto& it : m_apTableList )
    {
        // очищаем
        it->TableDelete();

        // удаляем
        delete it;
    }

    // убираем все ранее созданное (например, лейбл Таблицы)
    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    clear();

    widget_size_reset();

    widget_stretch( 0, 0 );
}

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

            std::string name;

            // ищем имя
            name = __yaml_GetString( config[ GoodsCategorySection ][j], GoodsCategoryName );
            pCategory->setCategoryName( name );

            // ищем ulink
            name = __yaml_GetString( config[ GoodsCategorySection ][j], GoodsUlinkSection );
            pCategory->setCategoryUlink( name );

            // ищем uname
            name = __yaml_GetString( config[ GoodsCategorySection ][j], GoodsUnameSection );
            pCategory->setCategoryUname( name );

            if( __yaml_IsSequence( config[ GoodsCategorySection ][j][ GoodsParametersSection ] ) )
            {
                TParam  *pParam;

                unsigned type;
                int height = m_vlayout->spacing();

                for( int i = 0; i < static_cast<int>(config[ GoodsCategorySection ][j][ GoodsParametersSection ].size()); i++ )
                {
                    pParam = new TParam( pCategory, Q_NULLPTR, 0 );
                    pParam->setNode( config[ GoodsCategorySection ][j][ GoodsParametersSection ][i] );
                    pParam->setNodeParent( config[ GoodsCategorySection ][j][ GoodsParametersSection ] );
                    pParam->setNodeIndex( i );

                    // добавляемся тут, внутри нельзя, т.к. getParameters вызывается разными классами
                    pCategory->m_vlayout->addWidget( pParam, 0, Qt::AlignLeft | Qt::AlignTop );
                    pCategory->m_apParamList.append( pParam );

                    YAML::Node  node_par = config[ GoodsCategorySection ][j][ GoodsParametersSection ][i];

                    pCategory->getParameters( node_par, pParam, pParam->getParamDepth() );

                    type = pParam->getParamType();

                    if( ( 1 == type ) || ( 2 == type ) || ( 3 == type ) )
                    {
                        height += height;
                    }

                    widget_stretch( 0, height );
                }
            }
        }
    }

    // таблица (tables)
    if( __yaml_IsSequence( config[ GoodsTableSection ] ) )
    {
        // файл не пуст
        m_bEmpty = false;

        // рисуем лейбл Таблицы
        QLabel *label = new QLabel();
        label->setFixedWidth( 93 );
        label->setFixedHeight( 40 );
        label->setFrameStyle( QFrame::NoFrame );
        label->setText( "Таблицы" );
        m_vlayout->addWidget( label, 0, Qt::AlignLeft );

        TTable  *pTable;
        TTabEntry  *pEntry;

        // перебираем все таблицы
        for( int j = 0; j < static_cast<int>(config[ GoodsTableSection ].size()); j++ )
        {
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

            // определяем тип таблицы (ссылка, строка, столбец)

            std::string  link = __yaml_GetString( config[ GoodsTableSection ][j], GoodsTableLink );

            if( 0 != link.length() )
            {
                pTable->setTableType( TTable::keTypeLink );

                pTable->setTableLink( link );
            }
            else if( __yaml_IsSequence( config[ GoodsTableSection ][j][GoodsTableColumn] ) )
            {
                pTable->setTableType( TTable::keTypeColumn );

                int  max_row_count = 0;

                for( int i = 0; i < static_cast<int>(config[ GoodsTableSection ][j][ GoodsTableColumn ].size()); i++ )
                {
                    // новая запись
                    pEntry = new TTabEntry( pTable );
                    pEntry->setNode( config[ GoodsTableSection ][j][ GoodsTableColumn ][i] );
                    pEntry->setNodeParent( config[ GoodsTableSection ][j][ GoodsTableColumn ] );
                    pEntry->setNodeIndex( i );

                    // добавляем запись в список
                    pTable->m_apColumnList.append( pEntry );

                    // имя
                    std::string  col_name = __yaml_GetString( config[ GoodsTableSection ][j][ GoodsTableColumn ][i], GoodsTableName );
                    pEntry->setEntryName( col_name );

                    // значение
                    const std::string  col_val = __yaml_GetString( config[ GoodsTableSection ][j][ GoodsTableColumn ][i], GoodsTableValue );
                    //pEntry->setEntryValues( col_val );

                    QStringList  col_list;
                    col_list.clear();
                    col_list = QString::fromStdString(col_val).split( '\n', QString::SkipEmptyParts );
                    pEntry->setParamList( col_list, col_val );
                    pTable->setTableColumn( col_name, col_list );

                    if( col_list.size() > max_row_count )
                    {
                        max_row_count = col_list.size();
                    }
                }

                pTable->fixTableHeight( 2 + static_cast<unsigned>(max_row_count) );
            }
            else if( __yaml_IsSequence( config[ GoodsTableSection ][j][GoodsTableRow] ) )
            {
                pTable->setTableType( TTable::keTypeRow );

                unsigned  row_count = 0;

                for( int i = 0; i < static_cast<int>(config[ GoodsTableSection ][j][ GoodsTableRow ].size()); i++ )
                {
                    // новая запись
                    pEntry = new TTabEntry( pTable );
                    pEntry->setNode( config[ GoodsTableSection ][j][ GoodsTableRow ][i] );
                    pEntry->setNodeParent( config[ GoodsTableSection ][j][ GoodsTableRow ] );
                    pEntry->setNodeIndex( i );

                    // добавляем запись в список
                    pTable->m_apRowList.append( pEntry );

                    // имя
                    std::string  row_name = __yaml_GetString( config[ GoodsTableSection ][j][ GoodsTableRow ][i], GoodsTableName );
                    pEntry->setEntryName( row_name );

                    // значение
                    const std::string  row_val = __yaml_GetString( config[ GoodsTableSection ][j][ GoodsTableRow ][i], GoodsTableValue );
                    //pEntry->setEntryValues( row_val );

                    QStringList  row_list;
                    row_list.clear();
                    row_list = QString::fromStdString(row_val).split( '\n', QString::SkipEmptyParts );
                    pEntry->setParamList( row_list, row_val );
                    pTable->setTableRow( row_name, row_list );

                    row_count++;
                }

                if( row_count > 0 )
                {
                    pTable->fixTableHeight( row_count - 1 );
                }
            }
            else
            {
                pTable->setTableType( TTable::keTypeNone );
            }

            //widget_stretch( 0, m_vlayout->spacing() );
        }
    }

	return true;
}

//------------------------------------------------------------------------------

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
