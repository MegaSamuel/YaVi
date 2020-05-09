#ifndef PARAM_H
#define PARAM_H

#include <QtWidgets>

//------------------------------------------------------------------------------

class TParam : public QWidget
{
   Q_OBJECT

public:
    TParam();
    ~TParam();

    void      setParamName( const std::string&  name );
    void      setParamPlaceholder( const std::string&  name );
    void      setParamNew( const std::string&  name );
    void      setParamAfter( const std::string&  name );
    void      setParamBefore( const std::string&  name );
    void      setParamUlink( const std::string&  name );
    void      setParamUname( const std::string&  name );
    void      setParamMulti( const std::string&  name );

    void      setParamType( unsigned  val );
    void      setParamMin( unsigned  val );
    void      setParamMax( unsigned  val );

    QString   getParamName();
    QString   getParamPlaceholder();
    QString   getParamNew();
    QString   getParamAfter();
    QString   getParamBefore();
    QString   getParamUlink();
    QString   getParamUname();
    QString   getParamMulti();

    unsigned  getParamType();
    unsigned  getParamMin();
    unsigned  getParamMax();

    int       getParamWidth();
    int       getParamHeight();
protected Q_SLOTS :
    void      onBtnDec();
    void      onBtnInc();

private:
    void      clear();

    void      resetRow();
    void      resetColumn();
    void      nextRow();
    void      nextColumn();

    QString   m_zName;
    QString   m_zPlaceholder;
    QString   m_zNew;
    QString   m_zAfter;
    QString   m_zBefore;
    QString   m_zUlink;
    QString   m_zUname;
    QString   m_zMulti;

    unsigned  m_uType;
    unsigned  m_uMin;
    unsigned  m_uMax;

//    QGridLayout  *m_grid;
    QVBoxLayout  *m_vlayout;

    QPushButton  *m_ptBtnDec;
    QPushButton  *m_ptBtnInc;
    QLabel       *m_ptLblName;

    int       m_row;
    int       m_column;
};

//------------------------------------------------------------------------------

#endif // PARAM_H
