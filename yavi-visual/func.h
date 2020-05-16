#ifndef FUNC_H
#define FUNC_H

#include <yaml-cpp/yaml.h>

//------------------------------------------------------------------------------

inline bool __yaml_IsScalar( const YAML::Node&  node )
{
    return ( node.IsDefined() ? ( node.IsScalar() ? true : false ) : false );
}

inline bool __yaml_IsSequence( const YAML::Node&  node )
{
    return ( node.IsDefined() ? ( node.IsSequence() ? true : false ) : false );
}

inline bool __yaml_IsMap( const YAML::Node&  node )
{
    return ( node.IsDefined() ? ( node.IsMap() ? true : false ) : false );
}

inline const std::string __yaml_GetString( const YAML::Node&  node, const std::string&  name, const std::string  def = "" )
{
    if( __yaml_IsScalar( node[ name ] ) )
        return node[ name ].as<std::string>();
    return def;
}

//------------------------------------------------------------------------------

inline bool __yaml_SetScalar( YAML::Node&  node, const std::string&  name, const unsigned  val )
{
//    if( node.IsNull() )
//        return false;

    node[ name ] = val;

    return true;
}

inline bool __yaml_SetString( YAML::Node&  node, const std::string&  name, const std::string&  str )
{
//    if( node.IsNull() )
//        return false;

//    if( 0 == name.length() )
//        return false;

    node[ name ] = str;

    return true;
}

//------------------------------------------------------------------------------

inline bool __yaml_DelNode( YAML::Node&  parent, YAML::Node&  child )
{
    if( parent.IsNull() )
        return false;

    if( child.IsNull() )
        return false;

    return parent.remove( child );
}

//------------------------------------------------------------------------------

#endif // FUNC_H
