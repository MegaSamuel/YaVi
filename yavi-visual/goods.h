#ifndef GOODS_H
#define GOODS_H

#include <QtWidgets>

#include <stdio.h>

#include <string>
#include <yaml-cpp/yaml.h>

#include "category.h"
#include "table.h"

//------------------------------------------------------------------------------
/*
class TParameters : public QObject
{
public:
    TParameters(QWidget  *parent);
    ~TParameters();

    TValues  m_value;
};

//------------------------------------------------------------------------------

class TCategories : public QObject
{
public:
    TCategories(QWidget  *parent);
    ~TCategories();

    TValues  m_value;

    QList<TCategories>  m_categories;
    QList<TParameters>  m_parameters;
};

//------------------------------------------------------------------------------

class TCategory : public QObject
{
public:
    TCategory(QWidget  *parent);
    ~TCategory();

    TValues  m_value;

    QList<TCategories>  m_categories;
    QList<TParameters>  m_parameters;
};
*/
//------------------------------------------------------------------------------

class TGoodsPrivate;

//! класс - протокол взаимодействия, считанный с файла
class TGoods : public QWidget
{
    friend class TGoodsPrivate;

	public :
        // имена основных секций файла конфигурации
        #define  GoodsCategorySection    "category"
        #define  GoodsCategoryName       "name"

        #define  GoodsCategoriesSection  "categories"
        #define  GoodsParametersSection  "parameters"

        // имена основных секций таблицы файла конфигурации
        #define  GoodsTableSection       "tables"
        #define  GoodsTableId            "id"
        #define  GoodsTableName          "name"
        #define  GoodsTableValue         "values"
        #define  GoodsTableColumn        "colums"
        #define  GoodsTableRow           "rows"
        #define  GoodsTableLink          "link"

        // имена основных секций таблицы файла конфигурации
        #define  GoodsNameSection        "name"
        #define  GoodsTypeSection        "type"
        #define  GoodsPlaceholderSection "placeholder"
        #define  GoodsNewSection         "new"
        #define  GoodsAfterSection       "after"
        #define  GoodsBeforeSection      "before"
        #define  GoodsUlinkSection       "ulink"
        #define  GoodsUnameSection       "uname"
        #define  GoodsMinSection         "min"
        #define  GoodsMaxSection         "max"
        #define  GoodsMultiSection       "multi"
        #define  GoodsValuesSection      "values"

        // constructors
        TGoods();
        explicit TGoods( const YAML::Node&  config );  // создание из ноды yaml
        ~TGoods();

        // разбор протокола, считанного предварительно в yaml
        bool         parse_yaml( const YAML::Node&  config );

        void         clear() noexcept;
        bool         empty() const noexcept;
        size_t       size() const noexcept;

        int          get_table_size() noexcept;

        QVBoxLayout  *m_vlayout;

private :
        std::unique_ptr<TGoodsPrivate> 	priv__;

        int          m_w, m_h;  // размеры виджета
        void         fix_widget_size( int w, int h ) noexcept; // установка размеров виджета
};

//! тип - указатель на протокол, описанный в файле
typedef std::shared_ptr<TGoods> 	PTGoods;

//------------------------------------------------------------------------------

#endif // GOODS_H
