#ifndef TABENTRY_H
#define TABENTRY_H

#include <QtWidgets>

#include "values.h"
#include "table_dialog.h"
#include "table_entryvalue.h"

//------------------------------------------------------------------------------

class  TTable;

//------------------------------------------------------------------------------

class TTableEntry : public QWidget
{
   Q_OBJECT

public:
    explicit TTableEntry( TTable  *pAncestor = Q_NULLPTR );
    ~TTableEntry();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index )  noexcept;

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex()  noexcept;

    void           setEntryName( const std::string&  name, bool  set_to_node = false );
    QString        getEntryName() noexcept;

    void           setEntryValues( const std::string&  values, bool  set_to_node = false );
    QString        getEntryValues() noexcept;

    void           setEntryType( int ) noexcept;
    int            getEntryType() noexcept;

    // заменить запись в поле values
    void           renameParamList( QString&  item, int  index, bool  set_to_node = false );
    // убрать запись из поля values
    void           removeParamList( QString&  item, int  index, bool  set_to_node = false );
    // добавить запись в поле values
    void           addParamList( QString&  item, bool  set_to_node = false );
    // установить поле values
    void           setParamList( QStringList  vlist, const std::string&  list, bool  set_to_node = false );

    void           EntryDelete();

    QVBoxLayout   *m_vlayout;
    QHBoxLayout   *m_hlayout;

    QList<TTableEntryValue*>  m_apValueList;

Q_SIGNALS:
    void           sendAdd( bool );
    void           sendName( QString&, int );

public Q_SLOTS:
    void           onBtnDec();
    void           onBtnName();
    void           onBtnInc();
    void           onSendEntryValue( QString&, int );

private Q_SLOTS:
    void           onSendCancel();
    void           onSendValue( QString& );

private:
    void           clear() noexcept;

    void           clearNodeSequence();

    void           makeStrByList() noexcept;

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    int            m_entry_type;  //

    QString        m_zName;
    QString        m_zValues;

    std::string    m_zList;
    QStringList    m_vList;

    TTabDialog    *m_ptTabDialogAdd;
    TTabDialog    *m_ptTabDialogName;

    TTable        *m_pAncestor;

    bool           need_to_add; // необходимость создать новое значение в ямле
};

//------------------------------------------------------------------------------

#endif // TABENTRY_H
