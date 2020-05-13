#ifndef VALUES_H
#define VALUES_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

//------------------------------------------------------------------------------

class TValues
{
public:
    TValues()
    {
        m_zName.clear();
        m_uType = 0;
        m_zPlaceholder.clear();
        m_zNew.clear();
        m_zAfter.clear();
        m_zBefore.clear();
        m_zUlink.clear();
        m_zUname.clear();
        m_uMin = 0;
        m_uMax = 0;
        m_zMulti.clear();

        m_vList.clear();
    }
    ~TValues()
    {

    }

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

    QString      m_zName;
    unsigned     m_uType;
    QString      m_zPlaceholder;
    QString      m_zNew;
    QString      m_zAfter;
    QString      m_zBefore;
    QString      m_zUlink;
    QString      m_zUname;
    unsigned     m_uMin;
    unsigned     m_uMax;
    QString      m_zMulti;

    QStringList  m_vList;
};

Q_DECLARE_METATYPE( TValues )

//------------------------------------------------------------------------------

#endif // VALUES_H
