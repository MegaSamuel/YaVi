#include <assert.h>
#include <initializer_list>
#include <iostream>
#include <signal.h>

#include "work.h"

//------------------------------------------------------------------------------

void TWork::draw()
{
    QPushButton *button1 = new QPushButton("One");
    QPushButton *button2 = new QPushButton("Two");
    QPushButton *button3 = new QPushButton("Three");
    m_hlayout->addWidget(button1);
    m_hlayout->addWidget(button2);
    m_hlayout->addWidget(button3);
}

void TWork::undraw()
{
    while( QLayoutItem* item = m_hlayout->takeAt(0) )
    {
        delete item->widget();
        delete item;
    }
}

//------------------------------------------------------------------------------

TWork::TWork()
{
    m_fBeginTime = cpu_time();

    m_pGoods = new TGoods;

    // создаем и запускаем основной таймер
    m_uTimerCounter = 0;
    m_ptTimer = new QTimer( this );
    connect( m_ptTimer, SIGNAL(timeout()), this, SLOT(TimerWork()) );
    m_ptTimer->start(1000); // 1 Hz

    m_hlayout = new QHBoxLayout;
    m_hlayout->addWidget( m_pGoods );
    this->setLayout( m_hlayout );
}

TWork::~TWork()
{

}

//------------------------------------------------------------------------------

double TWork::cpu_time() const
{
    return QTime::currentTime().msecsSinceStartOfDay()/1e3 - 0.0;
}

double TWork::current_time() const
{
    return cpu_time() - m_fBeginTime;
}

void TWork::TimerWork()
{
    m_uTimerCounter = ( m_uTimerCounter + 1 ) & 3;

//    qDebug() << current_time() << __func__ << __LINE__ << "->" << "counter" << m_uTimerCounter;

//    qDebug() << current_time() << __func__ << __LINE__ << "->" << m_pGoods->get_table_size();
}

//------------------------------------------------------------------------------

bool  TWork::init( const QString&  filename )
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

    in.setCodec("UTF-8");

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
    m_pGoods->parse_yaml( config );

    // возможно ямл пустой
    if( m_pGoods->empty() )
    {
        zFailReason = "file is empty";
        return false;
    }

    return true;
}

bool  TWork::fini( const QString&  filename )
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
