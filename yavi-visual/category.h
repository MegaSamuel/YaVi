#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtWidgets>

#include "dialog.h"
#include "param.h"
#include "values.h"

//------------------------------------------------------------------------------

class  TGoods;

//------------------------------------------------------------------------------

class TCategory : public QWidget
{
    Q_OBJECT

public:
    TCategory( TGoods  *pAncestor = Q_NULLPTR );
    ~TCategory();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

    void           getCategories( const YAML::Node&  node, TCategories  *a_pCategories, int  depth );
    void           getParameters( const YAML::Node&  node, TParam  *a_pParam, int  depth );

    int            getCategoryWidth();  // вернуть ширину
    int            getCategoryHeight(); // вернуть высоту

    void           setCategoryName( const std::string&  name, bool  set_to_node = false );
    const QString  getCategoryName();

    void           CategoryDelete();

    void           widget_stretch( int width, int height ) noexcept;         // растягиваем виджет
    void           widget_parent_stretch( int width, int height ) noexcept;  // растягиваем виджет

    void           widget_shrink( int width, int height ) noexcept;          // сжимаем виджет
    void           widget_parent_shrink( int width, int height ) noexcept;   // сжимаем виджет

    QVBoxLayout   *m_vlayout;   // главный layout класса
    QList<TParam*> m_apParamList;

protected Q_SLOTS :
    void           onBtnName();
    void           onBtnInc();
    void           onSendCancel();
    void           onSendValues( TValues& );

private:
    void           clear();

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    //QVBoxLayout   *m_vlayout;   // главный layout класса
    QHBoxLayout   *m_hlayout;   // вложенный layout

    QPushButton   *m_ptBtnName; // кнопка "Имя"
    QPushButton   *m_ptBtnInc;  // кнопка "+"

    QString        m_zName;     // category Name
    QString        m_zBtnName;  // текст на кнопке "Имя"

    int            m_depth;     // глубина вложения

    TDialog       *m_ptDialog;  // диалог на кнопке "Имя"

    TValues        m_tValues;

    TGoods        *m_pAncestor;

    int            m_width;     // ширина виджета
    int            m_height;    // высота виджета

    void           widget_size_reset() noexcept;  // сброс размера виджета

    bool           need_to_add; // необходимость создать новый набор параметров в ямле
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
