#include "tabrow.h"

//------------------------------------------------------------------------------

TTabRow::TTabRow()
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;
}

TTabRow::~TTabRow()
{

}

//------------------------------------------------------------------------------

void  TTabRow::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTabRow::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TTabRow::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTabRow::getNode()
{
    return m_node;
}

YAML::Node&  TTabRow::getNodeParent()
{
    return m_node_parent;
}

int  TTabRow::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------
