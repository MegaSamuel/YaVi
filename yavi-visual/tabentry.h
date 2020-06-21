#ifndef TABENTRY_H
#define TABENTRY_H

#include <QtWidgets>

#include "values.h"
#include "tabdialog.h"

//------------------------------------------------------------------------------

class TTabEntry : public QWidget
{
   Q_OBJECT

public:
    explicit TTabEntry();
    ~TTabEntry();

    void           setNode( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    int            getNodeIndex();

    void           setEntryName( const std::string&  name, bool  set_to_node = false );
    void           setEntryValues( const std::string&  name, bool  set_to_node = false );

    QString        getEntryName();
    QString        getEntryValues();

Q_SIGNALS:
    void           sendName( QString& name );

public Q_SLOTS:
    void           onBtnDec();
    void           onBtnName();

private Q_SLOTS:
    void           onSendCancel();
    void           onSendValue( QString& );

private:
    YAML::Node     m_node;        // текущий уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    QString        m_zName;
    QString        m_zValues;

    TTabDialog    *m_ptTabDialogName;
};

//------------------------------------------------------------------------------

#endif // TABENTRY_H
