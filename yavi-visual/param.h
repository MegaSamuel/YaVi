#ifndef PARAM_H
#define PARAM_H

#include <QtWidgets>

#include "dialog.h"
#include "values.h"

//------------------------------------------------------------------------------

class TCategory;

class TParam;

//------------------------------------------------------------------------------

class TCategories : public QWidget
{
   Q_OBJECT

public:
    TCategories( TParam  *pMentor = Q_NULLPTR, int  depth = 0 );
    ~TCategories();

    void           setCategoriesName( const std::string&  name );
    QString        getCategoriesName();

    int            getCategoriesDepth();

    void           CategoriesDelete();

    QList<TParam*> m_apParamList;

protected Q_SLOTS :
    void           onBtnDec();
    void           onBtnInc();
    void           onBtnName();
    void           onSendValues( TValues& );

private:
    void           clear();

    QVBoxLayout   *m_vlayout;

    QPushButton   *m_ptBtnDec;
    QPushButton   *m_ptBtnInc;
    QPushButton   *m_ptBtnName;

    QString        m_zName;     // categories Name
    QString        m_zBtnName;  // текст на кнопке

    int            m_depth;

    TDialog       *m_ptDialog;

    TValues        m_tValues;

    TParam        *m_pMentor;
};

//------------------------------------------------------------------------------


class TParam : public QWidget
{
   Q_OBJECT

public:
    TParam( TCategory  *pAncestor = Q_NULLPTR, TCategories  *pMentor = Q_NULLPTR, int  depth = 0 );
    ~TParam();

    void           setParamName( const std::string&  name );
    void           setParamPlaceholder( const std::string&  name );
    void           setParamNew( const std::string&  name );
    void           setParamAfter( const std::string&  name );
    void           setParamBefore( const std::string&  name );
    void           setParamUlink( const std::string&  name );
    void           setParamUname( const std::string&  name );
    void           setParamMulti( const std::string&  name );

    void           setParamType( unsigned  val );
    void           setParamMin( unsigned  val );
    void           setParamMax( unsigned  val );

    void           setParamList( QStringList  list );

    QString        getParamName();
    QString        getParamPlaceholder();
    QString        getParamNew();
    QString        getParamAfter();
    QString        getParamBefore();
    QString        getParamUlink();
    QString        getParamUname();
    QString        getParamMulti();

    unsigned       getParamType();
    unsigned       getParamMin();
    unsigned       getParamMax();

    QStringList    getParamList();

    int            getParamWidth();
    int            getParamHeight();

    void           ParamDelete();

    QList<TCategories*>  m_apCategoriesList;

protected Q_SLOTS :
    void           onBtnDec();
    void           onBtnInc();
    void           onBtnName();
    void           onSendValues( TValues& );

private:
    void           clear();

    QString        m_zName;
    QString        m_zPlaceholder;
    QString        m_zNew;
    QString        m_zAfter;
    QString        m_zBefore;
    QString        m_zUlink;
    QString        m_zUname;
    QString        m_zMulti;

    unsigned       m_uType;
    unsigned       m_uMin;
    unsigned       m_uMax;

    QStringList    m_vList;

    QVBoxLayout   *m_vlayout;

    QPushButton   *m_ptBtnDec;
    QPushButton   *m_ptBtnInc;
    QPushButton   *m_ptBtnName;

    QString        m_zBtnName;  // текст на кнопке

    TDialog       *m_ptDialog;

    TValues        m_tValues;

    TCategory     *m_pAncestor;
    TCategories   *m_pMentor;
};

//------------------------------------------------------------------------------

#endif // PARAM_H
