#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtWidgets>

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

    void  setCategoryId( const std::string&  name );
    void  setCategoryName( const std::string&  name );

    const QString getCategoryId();
    const QString getCategoryName();

protected Q_SLOTS :
    void    onBtnDec();
    void    onBtnInc();

private:
    QGridLayout  *m_grid;

    QPushButton  *m_ptBtnDec;
    QPushButton  *m_ptBtnInc;
    QLabel       *m_ptLblName;

    QString       m_zId;    // category Id
    QString       m_zName;  // category Name

    int           m_row;
    int           m_column;
};

//------------------------------------------------------------------------------

#endif // CATEGORY_H
