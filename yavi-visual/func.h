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

#endif // FUNC_H
