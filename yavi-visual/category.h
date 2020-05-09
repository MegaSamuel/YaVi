#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

#include "dialog.h"
#include "param.h"

//------------------------------------------------------------------------------

class TCategory : public QWidget
{
    Q_OBJECT

public:
    TCategory();
    ~TCategory();

    void           getCategories( const YAML::Node&  node, TParam  *a_pParam, int  depth );
    void           getParameters( const YAML::Node&  node, TParam  *a_pParam, int  depth );

    int            getCategoryWidth();  // вернуть ширину
    int            getCategoryHeight(); // вернуть высоту

    void           setCategoryName( const std::string&  name );
    const QString  getCategoryName();

protected Q_SLOTS :
    void           onBtnName();
    void           onBtnInc();

private:
    QVBoxLayout    *m_vlayout;

    QPushButton    *m_ptBtnName;
    QPushButton    *m_ptBtnInc;

    QString         m_zName;    // category Name
    QString         m_zBtnName; // текст на кнопке

    int             m_depth;    // глубина вложения

    QList<TParam*>  m_apParamList;

    TDialog        *m_ptDialog;
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
