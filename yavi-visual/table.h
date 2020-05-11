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

    void           setTableId( const std::string&  name );
    void           setTableName( const std::string&  name );
    void           setTableLink( const std::string&  name );
    void           setTableRow( QStringList& list );
    void           setTableColumn( QStringList& list );

    void           resetRow();
    void           resetColumn();
    void           nextRow();
    void           nextColumn();

    int            getTableWidth();
    int            getTableHeight();

    const QString  getTableId();
    const QString  getTableName();
    const QString  getTableLink();

protected Q_SLOTS :
    void           onBtnName();
    void           onBtnInc();

private:
    QGridLayout   *m_grid;

    QPushButton   *m_ptBtnInc;
    QPushButton   *m_ptBtnName;

    QLabel        *m_ptLblLink;

    QString        m_zId;      // table Id
    QString        m_zName;    // table Name
    QString        m_zBtnName; // текст на кнопке
    QString        m_zLink;    // table Link

    std::vector<std::string>  m_vValues;

    int            m_row;
    int            m_column;

    inline void  clear() noexcept
    {
        m_vValues.clear();
    }
};

//------------------------------------------------------------------------------

#endif // TABLE_H
