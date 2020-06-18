#ifndef TABROW_H
#define TABROW_H

#include <QtWidgets>

#include "values.h"

//------------------------------------------------------------------------------

class TTabRow : public QWidget
{
   Q_OBJECT

public:
    explicit TTabRow();
    ~TTabRow();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

private:
    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    YAML::Node     m_temporary_node;  // временный ямл для правки основного
    YAML::Node     m_temporary_inner_node;  // временный ямл для правки основного
};

//------------------------------------------------------------------------------

#endif // TABROW_H
