#ifndef TABENTRY_H
#define TABENTRY_H

#include <QtWidgets>

#include "values.h"
#include "tabdialog.h"
#include "tabentryvalue.h"

//------------------------------------------------------------------------------

class TTable;

//------------------------------------------------------------------------------

class TTabEntry : public QWidget
{
   Q_OBJECT

public:
    explicit TTabEntry( TTable  *pAncestor );
    ~TTabEntry();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

    void           setEntryName( const std::string&  name, bool  set_to_node = false );
    void           setEntryValues( const std::string&  name, bool  set_to_node = false );

    QString        getEntryName();
    QString        getEntryValues();

    void           EntryDelete();

    QList<TTabEntryValue*>  m_apValueList;

Q_SIGNALS:
    void           sendName( QString& name );

public Q_SLOTS:
    void           onBtnDec();
    void           onBtnName();

private Q_SLOTS:
    void           onSendCancel();
    void           onSendValue( QString& );

private:
    void           clearNodeSequence();

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    TTable        *m_pAncestor;

    QString        m_zName;
    QString        m_zValues;

    TTabDialog    *m_ptTabDialogName;
};

//------------------------------------------------------------------------------

#endif // TABENTRY_H
