#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

#include "dialog.h"
#include "param.h"
#include "values.h"

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

    QList<TParam*>  m_apParamList;

protected Q_SLOTS :
    void           onBtnName();
    void           onBtnInc();
    void           onSendValues( TValues& );

private:
    QVBoxLayout    *m_vlayout;

    QPushButton    *m_ptBtnName;
    QPushButton    *m_ptBtnInc;

    QString         m_zName;    // category Name
    QString         m_zBtnName; // текст на кнопке

    int             m_depth;    // глубина вложения

    TDialog        *m_ptDialog;

    TValues         m_tValues;
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
