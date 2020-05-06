#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets>

//------------------------------------------------------------------------------

class TTable : public QWidget
{
   Q_OBJECT

public:
    TTable();
    ~TTable();

    void  setTableId( const std::string&  name );
    void  setTableName( const std::string&  name );
    void  setTableLink( const std::string&  name );
    void  setTableRow( QStringList& list );
    void  setTableColumn( QStringList& list );

    void  resetRow();
    void  resetColumn();
    void  nextRow();
    void  nextColumn();

    int   getTableWidth();
    int   getTableHeight();

    const QString getTableId();
    const QString getTableName();
    const QString getTableLink();

protected Q_SLOTS :
    void    onBtnDec();
    void    onBtnInc();

private:
    QGridLayout  *m_grid;

    QPushButton  *m_ptBtnDec;
    QPushButton  *m_ptBtnInc;
    QLabel       *m_ptLblName;

    QLabel       *m_ptLblLink;

//    QList<QVector<QLabel*>>  m_apRowList;
//    QList<QVector<QLabel*>>  m_apColumnList;

    QString       m_zId;    // table Id
    QString       m_zName;  // table Name
    QString       m_zLink;  // table Link

    std::vector<std::string>  m_vValues;

    int m_row;
    int m_column;

    inline void  clear() noexcept
    {
        m_vValues.clear();
    }
};

//------------------------------------------------------------------------------

#endif // TABLE_H
