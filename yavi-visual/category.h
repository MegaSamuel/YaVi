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
    YAML::Node&    getNode();

    void           getCategories( const YAML::Node&  node, TParam  *a_pParam, int  depth );
    void           getParameters( const YAML::Node&  node, TParam  *a_pParam, int  depth );

    int            getCategoryWidth();  // вернуть ширину
    int            getCategoryHeight(); // вернуть высоту

    void           setCategoryName( const std::string&  name, bool  set_to_node = false );
    const QString  getCategoryName();

    void           CategoryDelete();

//    void           drawParam( TParam *pParam );

    QList<TParam*> m_apParamList;

protected Q_SLOTS :
    void           onBtnName();
    void           onBtnInc();
    void           onSendCancel();
    void           onSendValues( TValues& );

private:
    YAML::Node     m_node;      // текущий уровнь дерева ямла

    QVBoxLayout   *m_vlayout;

    QPushButton   *m_ptBtnName; // кнопка "Имя"
    QPushButton   *m_ptBtnInc;  // кнопка "+"

    QString        m_zName;     // category Name
    QString        m_zBtnName;  // текст на кнопке "Имя"

    int            m_depth;     // глубина вложения

    TDialog       *m_ptDialog;  // диалог на кнопке "Имя"

    TValues        m_tValues;

    TGoods        *m_pAncestor;

    bool           need_to_add; // необходимость создать новый набор параметров в ямле
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
