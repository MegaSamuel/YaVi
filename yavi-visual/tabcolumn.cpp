#include "tabcolumn.h"

//------------------------------------------------------------------------------

TTabColumn::TTabColumn()
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;
}

TTabColumn::~TTabColumn()
{

}

//------------------------------------------------------------------------------

void  TTabColumn::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTabColumn::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TTabColumn::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTabColumn::getNode()
{
    return m_node;
}

YAML::Node&  TTabColumn::getNodeParent()
{
    return m_node_parent;
}

int  TTabColumn::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------
