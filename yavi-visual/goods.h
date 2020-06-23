#ifndef GOODS_H
#define GOODS_H

#include <QtWidgets>

#include "category.h"
#include "table.h"

//------------------------------------------------------------------------------

//! класс - протокол взаимодействия, считанный с файла
class TGoods : public QWidget
{
    Q_OBJECT
public :
    explicit TGoods();
    ~TGoods();

    bool          parse_yaml( const YAML::Node&  config );

    bool          empty() const noexcept;

    QVBoxLayout  *m_vlayout;

    void          GoodsDelete();

    QStringList     collectULinks();

    void          widget_stretch( int width, int height ) noexcept;  // растягиваем виджет
    void          widget_shrink( int width, int height ) noexcept;   // сжимаем виджет

    QList<TTable*>     m_apTableList;
    QList<TCategory*>  m_apCategoryList;
    QLabel              *m_ptSeparator;

private :
    void          clear() noexcept;

    bool          m_bEmpty;

    int           m_width;     // ширина виджета
    int           m_height;    // высота виджета

    int           getWidgetWidth() noexcept;  // вернуть ширину
    int           getWidgetHeight() noexcept; // вернуть высоту

    void          widget_size_reset() noexcept; // сброс размера виджета
};

//------------------------------------------------------------------------------

#endif // GOODS_H
