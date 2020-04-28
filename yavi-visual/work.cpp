#include <assert.h>
#include <initializer_list>
#include <iostream>
#include <signal.h>

#include "work.h"

using namespace cobu;

//------------------------------------------------------------------------------

Work::Work()
{

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
        zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // разбираем ямл
    m_tGoods.parse_yaml( config );

    // возможно ямл пустой
    if( m_tGoods.empty() )
    {
        zFailReason = "file is empty";
        return false;
    }

#if 1
    QString filenameout = "D:/test.yml";
    std::string str;
    QFile 		fpout( filenameout );

    if( !fpout.open( QIODevice::ReadWrite | QIODevice::Text ) )
    {
        qDebug() << "Cannot open file " << filenameout;
        return false;
    }

    QTextStream  out( &fpout );

    str = YAML::Dump( config );

//    out << str;
#endif

    return true;
}

bool  Work::fini( const QString&  filename )
{
    YAML::Node 	config;
    QFile 		fp( filename );

    // открываем файл на запись
    if( !fp.open( QIODevice::ReadWrite | QIODevice::Text ) )
    {
        qDebug() << "Cannot open file " << filename;
        zFailReason = "Cannot open file " + filename;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
