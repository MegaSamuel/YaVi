#include <assert.h>
#include <initializer_list>
#include <iostream>
#include <signal.h>

#include "work.h"

using namespace cobu;

//------------------------------------------------------------------------------

//Work::Work( const QString &filename, QObject  *parent )
Work::Work()
{
//    bInit = init( filename );
}

Work::~Work()
{

}

//------------------------------------------------------------------------------

bool  Work::init( const QString&  filename )
{
    YAML::Node 	config;
    QFile 		fp( filename );

    // а есть ли файл?
    if( !fp.exists() )
    {
        qDebug() << "File" << filename << "is not exists";
        return false;
    }

    // открываем файл только на чтение
    if( !fp.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Cannot open file " << filename;
        return false;
    }

    QTextStream  in( &fp );

    // пробуем читать ямл
    try {
        config = YAML::Load( in.readAll().toStdString() );
    } catch ( const YAML::Exception&  e ) {
        // что-то пошло не так, а что смотрим в e.what()
//        qDebug() << "Exception of parse file" << filename << ":" << e.what();
        zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // разбираем ямл
    m_tGoods.parse_yaml( config );

    // возможно ямл пустой
    if( m_tGoods.empty() )
        return false;

    return true;
}

//------------------------------------------------------------------------------
