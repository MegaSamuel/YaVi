#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets>

#include "tabdialog.h"
#include "values.h"

//------------------------------------------------------------------------------

class  TGoods;

//------------------------------------------------------------------------------


class TTable : public QWidget
{
   Q_OBJECT

public:
    explicit TTable( TGoods  *pAncestor = Q_NULLPTR );
    ~TTable();

    enum ETableType
    {
        keTypeNone   = 0,
        keTypeLink   = 1,
        keTypeColumn = 2,
        keTypeRow    = 3,
    };

    void           setTableType( unsigned  type ) noexcept;

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

    void           setTableId( const std::string&  name, bool  set_to_node = false );
    void           setTableName( const std::string&  name, bool  set_to_node = false );
    void           setTableLink( const std::string&  name, bool  set_to_node = false );
    void           setTableRow( const std::string&  name, QStringList& list );
    void           setTableColumn( const std::string&  name, QStringList& list );

    void           fixTableHeight( unsigned  row_count );

    void           resetRow() noexcept;
    void           resetColumn() noexcept;
    void           nextRow() noexcept;
    void           nextColumn() noexcept;

    int            getTableWidth() noexcept;
    int            getTableHeight() noexcept;

    const QString  getTableId();
    const QString  getTableName();
    const QString  getTableLink();

    void           TableDelete();

    void           widget_stretch( int width, int height, bool add_height = true ) noexcept;         // растягиваем виджет
    void           widget_shrink( int width, int height ) noexcept;          // сжимаем виджет

Q_SIGNALS:
    void           sendChanged();

private Q_SLOTS:
    void           onBtnId();
    void           onBtnName();
    void           onBtnLink();
    void           onBtnDec();
    void           onBtnInc();
    void           onBtnRowInc();
    void           onBtnRowName();
    void           onBtnRowValInc();
    void           onBtnRowValName();
    void           onBtnColumnInc();
    void           onBtnColumnName();
    void           onBtnColumnValInc();
    void           onBtnColumnValName();
    void           onSendCancel();
    void           onSendValue( QString& );
    void           onSendValues( TValues& );

private:
    void           clear();

    void           clearNodeSequence();

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    YAML::Node     m_temporary_node;  // временный ямл для правки основного
    YAML::Node     m_temporary_inner_node;  // временный ямл для правки основного

    QGridLayout   *m_grid;

    QPushButton   *m_ptBtnInc;
    QPushButton   *m_ptBtnId;
    QPushButton   *m_ptBtnName;
    QPushButton   *m_ptBtnLink;

    unsigned       m_uTableType;

    QString        m_zId;      // table Id
    QString        m_zBtnId;   // текст на кнопке
    QString        m_zName;    // table Name
    QString        m_zBtnName; // текст на кнопке
    QString        m_zLink;    // table Link
    QString        m_zBtnLink; // текст на кнопке

    TTabDialog    *m_ptTabDialogId;
    TTabDialog    *m_ptTabDialogName;
    TTabDialog    *m_ptTabDialogLink;
    TTabDialog    *m_ptTabDialogAdd;

    TValues        m_tValues;

    int            m_row;
    int            m_column;

    TGoods        *m_pAncestor;

    int            m_width;     // ширина виджета
    int            m_height;    // высота виджета

    void           widget_parent_stretch( int width, int height, bool add_height = true ) noexcept;  // растягиваем виджет
    void           widget_parent_shrink( int width, int height ) noexcept;   // сжимаем виджет

    void           widget_size_reset() noexcept;  // сброс размера виджета

    bool           need_to_add; // необходимость создать новый набор параметров в ямле

    bool           edit_id;
    bool           edit_name;
    bool           edit_link;
};

//------------------------------------------------------------------------------

#endif // TABLE_H
