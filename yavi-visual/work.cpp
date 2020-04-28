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

    // сохраняем ямл
    m_config = config;

    // разбираем ямл
    m_tGoods.parse_yaml( config );

    // возможно ямл пустой
    if( m_tGoods.empty() )
    {
        zFailReason = "file is empty";
        return false;
    }

    return true;
}

bool  Work::fini( const QString&  filename )
{
    std::string str;
    QFile 		fp( filename );
    QTextStream out( &fp );

    // открываем файл на запись
    if( !fp.open( QIODevice::ReadWrite | QIODevice::Text ) )
    {
        qDebug() << "Cannot create file " << filename;
        zFailReason = "Cannot create file " + filename;
        return false;
    }

    // пробуем выгрузить ямл в строку
    try {
        str = YAML::Dump( m_config );
    } catch ( const YAML::Exception&  e ) {
        // что-то пошло не так, а что смотрим в e.what()
        zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // пишем строку в файл
    out.setCodec("UTF-8");
    out << QString::fromStdString( str );

    fp.close();

    return true;
}

//------------------------------------------------------------------------------
