#include "tabentry.h"

//------------------------------------------------------------------------------

TTabEntry::TTabEntry()
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;
}

TTabEntry::~TTabEntry()
{

}

//------------------------------------------------------------------------------

void  TTabEntry::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTabEntry::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TTabEntry::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTabEntry::getNode()
{
    return m_node;
}

YAML::Node&  TTabEntry::getNodeParent()
{
    return m_node_parent;
}

int  TTabEntry::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------
