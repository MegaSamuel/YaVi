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
//    void           setEntryValues( const std::string&  name, bool  set_to_node = false );

    QString        getEntryName();
//    QString        getEntryValues();

    // заменить запись в поле values
    void           renameParamList( QString&  item, int  index, bool  set_to_node = false );
    // убрать запись из поля values
    void           removeParamList( QString&  item, int  index, bool  set_to_node = false );
    // добавить запись в поле values
    void           addParamList( QString&  item, bool  set_to_node = false );
    // установить поле values
    void           setParamList( QStringList  vlist, const std::string&  list, bool  set_to_node = false );

    void           EntryDelete();

    QList<TTabEntryValue*>  m_apValueList;

Q_SIGNALS:
    void           sendName( QString& name );

public Q_SLOTS:
    void           onBtnDec();
    void           onBtnName();
    void           onSendEntryValue( QString&, int );

private Q_SLOTS:
    void           onSendCancel();
    void           onSendValue( QString& );

private:
    void           clearNodeSequence();

    void           makeStrByList() noexcept;

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    TTable        *m_pAncestor;

    QString        m_zName;
    QString        m_zValues;

    std::string    m_zList;
    QStringList    m_vList;

    TTabDialog    *m_ptTabDialogName;
};

//------------------------------------------------------------------------------

#endif // TABENTRY_H
