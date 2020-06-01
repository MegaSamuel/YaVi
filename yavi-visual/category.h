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
    explicit TCategory( TGoods  *pAncestor = Q_NULLPTR );
    ~TCategory();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

    void           addCategories( YAML::Node& node, TParam  *a_pParam, const std::string&  name, int  depth, int  index );

    void           formCategories( const YAML::Node&  node, TParam  *a_pParam, int  depth );

    void           getCategories( YAML::Node&  node, TCategories  *a_pCategories, int  depth );
    void           getParameters( YAML::Node&  node, TParam  *a_pParam, int  depth );

    int            getCategoryWidth() noexcept;  // вернуть ширину
    int            getCategoryHeight() noexcept; // вернуть высоту

    void           setCategoryName( const std::string&  name, bool  set_to_node = false );
    void           setCategoryUlink( const std::string&  name, bool  set_to_node = false );
    void           setCategoryUname( const std::string&  name, bool  set_to_node = false );

    QString        getCategoryName() noexcept;
    QString        getCategoryUlink() noexcept;
    QString        getCategoryUname() noexcept;

    bool           isParamNameRedefined( const QString&  name );

    void           CategoryDelete();

    void           widget_stretch( int width, int height, bool add_height = true ) noexcept;         // растягиваем виджет
    void           widget_shrink( int width, int height ) noexcept;          // сжимаем виджет

    QVBoxLayout   *m_vlayout;   // главный layout класса
    QList<TParam*> m_apParamList;

Q_SIGNALS:
    void           sendChanged();

//private Q_SLOTS :
//    void           onBtnName();
//    void           onBtnInc();
//    void           onSendCancel();
//    void           onSendValues( TValues& );

private:
    void           onBtnName();
    void           onBtnInc();
    void           onSendCancel();
    void           onSendValues( TValues& );

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

    QString        m_zUlink;
    QString        m_zUname;

    int            m_depth;     // глубина вложения

    TDialog       *m_ptDialogSelf; // диалог на кнопке "Имя"
    TDialog       *m_ptDialogAdd;  // диалог на кнопке "+"

    TValues        m_tValues;

    TGoods        *m_pAncestor;

    int            m_width;     // ширина виджета
    int            m_height;    // высота виджета

    void           widget_parent_stretch( int width, int height, bool add_height = true ) noexcept;  // растягиваем виджет
    void           widget_parent_shrink( int width, int height ) noexcept;   // сжимаем виджет

    void           widget_size_reset() noexcept;  // сброс размера виджета

    bool           need_to_add; // необходимость создать новый набор параметров в ямле
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
