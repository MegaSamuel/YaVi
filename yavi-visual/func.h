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

inline unsigned __yaml_GetUnsigned( const YAML::Node&  node, const std::string&  name, unsigned  def = 0 )
{
    if( __yaml_IsScalar( node[ name ] ) )
        return node[ name ].as<unsigned>();
    return def;
}

inline int __yaml_GetInteger( const YAML::Node&  node, const std::string&  name, int  def = 0 )
{
    if( __yaml_IsScalar( node[ name ] ) )
        return node[ name ].as<int>();
    return def;
}

inline double __yaml_GetDouble( const YAML::Node&  node, const std::string&  name, double  def = 0.0 )
{
    if( __yaml_IsScalar( node[ name ] ) )
        return node[ name ].as<double>();
    return def;
}


//------------------------------------------------------------------------------

inline bool __yaml_SetScalar( YAML::Node&  node, const std::string&  name, const unsigned  val )
{
    node[ name ] = val;

    return true;
}

inline bool __yaml_SetInteger( YAML::Node&  node, const std::string&  name, const int  val )
{
    node[ name ] = val;

    return true;
}

inline bool __yaml_SetDouble( YAML::Node&  node, const std::string&  name, const double  val )
{
    node[ name ] = val;

    return true;
}

inline bool __yaml_SetString( YAML::Node&  node, const std::string&  name, const std::string&  str )
{
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
