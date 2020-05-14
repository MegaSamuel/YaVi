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
        bool           parse_yaml( const YAML::Node&  config );

        void           clear() noexcept;
        bool           empty() const noexcept;
        size_t         size() const noexcept;

        QVBoxLayout   *m_vlayout;

        void           GoodsDelete();

        QList<TTable*>     m_apTableList;
        QList<TCategory*>  m_apCategoryList;

private :
        std::unique_ptr<TGoodsPrivate> 	priv__;

        int            m_w, m_h;  // текущие размеры виджета
        void           widget_size_reset() noexcept; // сброс размера виджета
        void           widget_stretch( int w, int h ) noexcept; // растягиваем виджет
};

//! тип - указатель на протокол, описанный в файле
typedef std::shared_ptr<TGoods> 	PTGoods;

//------------------------------------------------------------------------------

#endif // GOODS_H
