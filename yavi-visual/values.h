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
        m_zId.clear();
        m_zName.clear();
        m_uType = 0;
        m_zPlaceholder.clear();
        m_zNew.clear();
        m_zAfter.clear();
        m_zBefore.clear();
        m_zUlink.clear();
        m_zUname.clear();
        m_nMin = 0;
        m_nMax = 0;
        m_fMin = 0.0;
        m_fMax = 0.0;
        m_zMulti.clear();

        m_zVal.clear();
        m_nVal = 0;
        m_fVal = 0.0;

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

    // набор параметров
    QString      m_zId;
    QString      m_zName;
    unsigned     m_uType;
    QString      m_zPlaceholder;
    QString      m_zNew;
    QString      m_zAfter;
    QString      m_zBefore;
    QString      m_zUlink;
    QString      m_zUname;
    int          m_nMin;
    int          m_nMax;
    double       m_fMin;
    double       m_fMax;
    QString      m_zMulti;

    // не используются
    QString      m_zVal; // значение (для type 1)
    int          m_nVal; // значение (для type 2)
    double       m_fVal; // значение (для type 3)

    // список для combobox (для type 4,5)
    QStringList  m_vList;
};

Q_DECLARE_METATYPE( TValues )

//------------------------------------------------------------------------------

#endif // VALUES_H
