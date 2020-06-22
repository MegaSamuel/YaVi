#ifndef TABENTRYVALUE_H
#define TABENTRYVALUE_H

#include <QtWidgets>

#include "values.h"
#include "tabdialog.h"

//------------------------------------------------------------------------------

class TTabEntry;

//------------------------------------------------------------------------------

class TTabEntryValue : public QWidget
{
   Q_OBJECT

public:
    explicit TTabEntryValue( TTabEntry  *pAncestor );
    ~TTabEntryValue();

    void           setNode( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    int            getNodeIndex();

    void           setValueIndex( int  index );
    int            getValueIndex();

    void           setEntryValue( QString&  value );

    QString        getEntryValue();

    void           EntryValueDelete();

Q_SIGNALS:
    void           sendEntryValue( QString&  value, int  index = 0 );

public Q_SLOTS:
    void           onBtnDec();
    void           onBtnValue();

private Q_SLOTS:
    void           onSendCancel();
    void           onSendValue( QString& );

private:
    YAML::Node     m_node;        // текущий уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    int            m_value_index;  // номер записи в values

    TTabEntry     *m_pAncestor;

    QString        m_zValue;

    TTabDialog    *m_ptTabDialogValue;
};

//------------------------------------------------------------------------------

#endif // TABENTRYVALUE_H
