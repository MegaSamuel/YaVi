#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

#include "param.h"

//------------------------------------------------------------------------------

class TCategory : public QWidget
{
    Q_OBJECT

public:
    TCategory();
    ~TCategory();

    void  resetRow();
    void  resetColumn();
    void  nextRow();
    void  nextColumn();

    int   getCategoryWidth();
    int   getCategoryHeight();

    void  setCategoryName( const std::string&  name );

    const QString getCategoryName();

protected Q_SLOTS :
    void    onBtnInc();

private:
    QGridLayout  *m_grid;

    QPushButton  *m_ptBtnInc;
    QLabel       *m_ptLblName;

    QString       m_zName;  // category Name

    int           m_row;
    int           m_column;

    QList<TParam*>  m_apParamList;
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
