#ifndef GOODS_H
#define GOODS_H

#include <QtWidgets>

#include "category.h"
#include "table.h"

//------------------------------------------------------------------------------

class TGoodsPrivate;

//! класс - протокол взаимодействия, считанный с файла
class TGoods : public QWidget
{
    friend class TGoodsPrivate;

	public :
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

        TCategory    *m_pCategory;
        TCategory    *getMasterPointer();

private :
        std::unique_ptr<TGoodsPrivate> 	priv__;

        int          m_w, m_h;  // размеры виджета
        void         fix_widget_size( int w, int h ) noexcept; // установка размеров виджета
};

//! тип - указатель на протокол, описанный в файле
typedef std::shared_ptr<TGoods> 	PTGoods;

//------------------------------------------------------------------------------

#endif // GOODS_H
