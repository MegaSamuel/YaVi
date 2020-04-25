#include <assert.h>
#include <initializer_list>
#include <iostream>
#include <signal.h>

#include "work.h"

Work::Work( const QString &filename, QObject  *parent )
{
    init( filename );
}

Work::~Work()
{

}

bool  Work::init( const QString&  filename )
{
    YAML::Node 		config;
    QFile 			fp( filename );

    if( ! fp.exists() )
    {
        qDebug() << "File" << filename << "is not exists";
        return false;
    }

    if( ! fp.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Cannot open file " << filename;
        return false;
    }
    QTextStream 	in( &fp );

    // read yaml-file
    try {
        config = YAML::Load( in.readAll().toStdString() );
    } catch ( const YAML::Exception&  e ) {
        qDebug() << "Exception of parse file" << filename << ":" << e.what();
        return false;
    }

    // parse yaml-file
//    m_tProtocol.parse_yaml( config );

//    if( m_tProtocol.empty() )
//        return false;

//    catchUnixSignals( { SIGQUIT, SIGINT, SIGTERM } );

    return true;
}
