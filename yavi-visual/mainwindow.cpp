#include "mainwindow.h"
#include "func.h"
#include "dialog.h"

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_fBeginTime = cpu_time();

    pMainWindow = this;

    m_zPrgName.clear();
    m_zPrgTitle.clear();
    m_bPrgTitleChanged = false;

    m_zPrgName = "YAML Visualizer";

    // создаем и запускаем основной таймер
    m_uTimerCounter = 0;
    m_ptTimer = new QTimer( this );
    connect( m_ptTimer, &QTimer::timeout, this, &MainWindow::onTimerWork );
    m_ptTimer->start(1000); // 1 Hz

    // ставим стиль
    QApplication::setStyle(QStyleFactory::create("fusion"));

    // заголовок формы
    setPrgTitleText();

    // иконка формы
    setWindowIcon( QIcon( ":/favicon.ico" ) );

    m_zFailReason.clear();
    m_config.reset();

    QFrame  *frmBase = new QFrame();

    QHBoxLayout  *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка "загрузить yaml из файла"
    m_ptBtnOpen = new QPushButton( "Open", this );
    connect( m_ptBtnOpen, &QPushButton::clicked, this, &MainWindow::onBtnOpen );
    hlayout->addWidget( m_ptBtnOpen, 0, Qt::AlignLeft );

    // кнопка "сохранить все в yaml"
    m_ptBtnSave = new QPushButton( "Save", this );
    connect( m_ptBtnSave, &QPushButton::clicked, this, &MainWindow::onBtnSave );
    hlayout->addWidget( m_ptBtnSave, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblNotice = new QLabel( this, Q_NULLPTR );
    //By default, the contents of the label are left-aligned and vertically-centered.
    //m_ptLblNotice->setAlignment( Qt::AlignLeft );
    m_ptLblNotice->setFrameStyle( QFrame::NoFrame );
    hlayout->addWidget( m_ptLblNotice, 0, Qt::AlignLeft );

    QVBoxLayout  *vlayout = new QVBoxLayout;
    vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // добавляем горизонтальный layout в вертикальный layout
    vlayout->addLayout( hlayout );

    // добавляем то где будет дерево ямла в вертикальный layout
    // ->
    QScrollArea *scroll = new QScrollArea;
    scroll->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_pGoods = new TGoods();
    scroll->setWidget( m_pGoods );

    vlayout->addWidget( scroll );
    // <-

    // добавляем вертикальный layout в frame
    frmBase->setLayout( vlayout );

    // центральный элеиент
    setCentralWidget( frmBase );

    // читаем ini и, возможно, что-то делаем
    actionAfterStart();
}

MainWindow::~MainWindow()
{

}

MainWindow *MainWindow::pMainWindow = nullptr;

MainWindow *MainWindow::getMainWinPtr()
{
    return pMainWindow;
}

//------------------------------------------------------------------------------

void  MainWindow::onBtnOpen()
{
    // очищаем лэйбл с описанием ошибки
    m_ptLblNotice->clear();
//    m_ptLblNotice->setStyleSheet( "QLabel {background-color: transparent;}" );

    // каталог где мы находимся
    QDir *pDir = new QDir( cfgGetLastOpenPath() );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators( QFileDialog::getOpenFileName( this, "Open file", dir, "YAML (*.yaml *.yml)" ) );

    if( !filename.isEmpty() )
    {
        // похоже что уже есть открытый ямл
        if( !m_config.IsNull() )
        {
            m_pGoods->GoodsDelete();

            m_config.reset();
        }

        if( false == init( filename ) )
        {
            QString  zReport;
            zReport = "Cannot open " + filename + " with result -> " + m_zFailReason;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
            // заголовок формы
            setPrgTitleText( filename );

            /*
            QString file = filename.section("\\",-1,-1);
            qDebug() << file;

            QString dir = filename.section("\\",0,-2);
            qDebug() << dir;
            */

            // запоминаем каталог
            cfgSetLastOpenPath( filename.section( QDir::separator(), 0, -2 ) );

            // запоминаем файл
            cfgSetLastOpenFile( filename );

            cfgRefresh();

//            qDebug() << "Open file" << filename << "separator" << QDir::separator();
        }
    }
    else
    {
        // долбоящер не ввел имя файла

        //qDebug() << "Cannot open: no filename";
    }
}

void  MainWindow::onBtnSave()
{
    // формируем имя файла по умолчанию
    QString deffilename = QString( "/test.yml" );

    // каталог где мы сохранялись крайний раз
    QDir *pDir = new QDir( cfgGetLastSavePath() + deffilename );

    // строка с именем каталога где мы сохранялись крайний раз
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators( QFileDialog::getSaveFileName( this, "Save file", dir, "YAML (*.yml)" ) );

    if( !filename.isEmpty() )
    {
        if( false == fini( filename ) )
        {
            QString  zReport;
            zReport = "Cannot write " + filename + " with result -> " + m_zFailReason;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
            m_bPrgTitleChanged = false;

            // заголовок формы
            setPrgTitleText( filename );

            // запоминаем каталог
            cfgSetLastSavePath( filename.section( QDir::separator(), 0, -2 ) );

            cfgRefresh();

//            qDebug() << "Save file" << filename;
        }
    }
    else
    {
        // долбоящер не ввел имя файла

//        qDebug() << "Cannot save: no filename";
    }
}

//------------------------------------------------------------------------------

double  MainWindow::cpu_time() const
{
    return QTime::currentTime().msecsSinceStartOfDay()/1e3 - 0.0;
}

double  MainWindow::current_time() const
{
    return cpu_time() - m_fBeginTime;
}

void  MainWindow::onTimerWork()
{
    m_uTimerCounter = ( m_uTimerCounter + 1 ) & 3;

//    qDebug() << current_time() << __func__ << __LINE__ << "->" << "counter" << m_uTimerCounter;

//    qDebug() << current_time() << __func__ << __LINE__ << "->" << m_pGoods->get_table_size();
}

//------------------------------------------------------------------------------

bool  MainWindow::init( const QString&  filename )
{
    QFile  fp( filename );

    // а есть ли файл?
    if( !fp.exists() )
    {
        m_zFailReason = "file is not exists";
        return false;
    }

    // открываем файл только на чтение
    if( !fp.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        m_zFailReason = "cannot open file";
        return false;
    }

    QTextStream  in( &fp );

    in.setCodec("UTF-8");

    // пробуем читать ямл
    try {
        m_config = YAML::Load( in.readAll().toStdString() );
    } catch ( const YAML::Exception&  e ) {
        // что-то пошло не так, а что смотрим в e.what()
        m_zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // разбираем ямл
    m_pGoods->parse_yaml( m_config );

    fp.close();

    // возможно ямл пустой
    if( m_pGoods->empty() )
    {
        m_zFailReason = "file is empty";
        return false;
    }

    return true;
}

bool  MainWindow::fini( const QString&  filename )
{
    std::string str;
    QFile 		fp( filename );
    QTextStream out( &fp );

    // открываем файл на запись
    if( !fp.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        m_zFailReason = "cannot create file";
        return false;
    }

/*
    YAML::Node node1;
    node1["name"] = "node 1";
    node1["type"] = 0;

    YAML::Node node2;
    node2["name"] = "node 2";
    node2["type"] = 0;

    YAML::Node node3;
    node3["name"] = "node 3";
    node3["type"] = 0;

    YAML::Node node4;
    node4["name"] = "node 4";
    node4["type"] = 0;

    YAML::Node node_name;
    node_name[ "name" ] = "NameStr";
    node_name[ "parameters" ].push_back(node1);
    node_name[ "parameters" ].push_back(node2);
    node_name[ "parameters" ].push_back(node3);
    node_name[ "parameters" ].push_back(node4);

    YAML::Node node_main;
    node_main["category"].push_back( node_name );

    qDebug() << "category count" << node_main["category"].size();

    qDebug() << QString::fromStdString(node_main["category"][0][ "name" ].as<std::string>());

    qDebug() << "parameters count" << node_main["category"][0]["parameters"].size();

    for( int i = 0; i < static_cast<int>(node_main["category"][0]["parameters"].size()); i++ )
    {
        qDebug() << i << QString::fromStdString(node_main["category"][0]["parameters"][i]["name"].as<std::string>());
    }

    // удаляем узел с именем node 2
    node_main["category"][0]["parameters"].remove(1);

    qDebug() << "parameters count" << node_main["category"][0]["parameters"].size();

    for( int i = 0; i < static_cast<int>(node_main["category"][0]["parameters"].size()); i++ )
    {
        qDebug() << i << QString::fromStdString(node_main["category"][0]["parameters"][i]["name"].as<std::string>());
    }
*/

    // пробуем выгрузить ямл в строку
    try {
        str = YAML::Dump( m_config );
//        str = YAML::Dump( node_main );
    } catch ( const YAML::Exception&  e ) {
        // что-то пошло не так, а что смотрим в e.what()
        m_zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // пишем строку в файл
    out.setCodec("UTF-8");
    out << QString::fromStdString( str );

    fp.close();

    return true;
}

//------------------------------------------------------------------------------

void  MainWindow::setPrgTitleText( const QString&  text )
{
    if( 0 != text.length() )
    {
        m_zPrgTitle = text + " - " + m_zPrgName;

        if( m_bPrgTitleChanged )
        {
            m_zPrgTitle.prepend( "*" );
        }
    }
    else
    {
        m_zPrgTitle = m_zPrgName;
    }

    setWindowTitle( m_zPrgTitle );
}

void  MainWindow::setPrgTitleChanged( bool  changed )
{
    if( changed )
    {
        m_zPrgTitle.prepend( "*" );

        setWindowTitle( m_zPrgTitle );
    }
    else
    {
        m_zPrgTitle.replace( "*", "" );

        setWindowTitle( m_zPrgTitle );
    }
}

void  MainWindow::onYamlChanged()
{
    // если признака что ямл изменился еще нет
    if( !m_bPrgTitleChanged )
    {
        // рисуем "*" перед именем файла
        setPrgTitleChanged( true );
    }

    // признак что ямл изменился
    m_bPrgTitleChanged = true;
}

//------------------------------------------------------------------------------

void  MainWindow::actionAfterStart()
{
    cfgReset();

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    cfgSetCurrentPath( dir );

    // формиреум путь с именем файла
    QString cfgfilename = QDir::toNativeSeparators( dir + "/yavi.yml" );

    if( false == cfgRead( cfgfilename ) )
    {
        // дефолтные значения
        cfgSetAutoload( 1 );
        cfgSetLastOpenPath();
        cfgSetLastSavePath();
        cfgSetLastOpenFile();

        // сохраняем
        cfgWrite( cfgfilename );
    }
    else
    {
        // сразу сохраняем
        cfgWrite( cfgfilename );
    }

    if( cfgGetAutoload() )
    {
        // полное имя ранее открытого файла
        QString filename = cfgGetLastOpenFile();

        if( !filename.isEmpty() )
        {
            if( init( filename ) )
            {
                // заголовок формы
                setPrgTitleText( filename );
            }
        }
    }
}

//------------------------------------------------------------------------------

void  MainWindow::cfgReset() noexcept
{
    m_cfg.reset();

    m_cfg_autoload = 1;

    m_cfg_last_open_path.clear();
    m_cfg_last_save_path.clear();
    m_cfg_last_open_file.clear();

    m_cfg_current_path.clear();
    m_cfg_filename.clear();
}

void  MainWindow::cfgRefresh() noexcept
{
    // формиреум путь с именем файла
    QString filename = QDir::toNativeSeparators( cfgGetCurrentPath() + "/yavi.yml" );

    // сохраняем
    cfgWrite( filename );
}

bool  MainWindow::cfgRead( const QString&  filename )
{
    QFile  fp( filename );

    // а есть ли файл?
    if( !fp.exists() )
    {
        m_zFailReason = "file is not exists";
        return false;
    }

    // открываем файл только на чтение
    if( !fp.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        m_zFailReason = "cannot open file";
        return false;
    }

    QTextStream  in( &fp );

    in.setCodec("UTF-8");

    // пробуем читать ямл
    try {
        m_cfg = YAML::Load( in.readAll().toStdString() );
    } catch ( const YAML::Exception&  e ) {
        m_zFailReason = QString::fromStdString( e.what() );
        fp.close();
        return false;
    }

    // разбираем ямл

    if( __yaml_IsScalar( m_cfg[ "autoload" ] ) )
    {
        try {
            cfgSetAutoload( m_cfg[ "autoload" ].as<unsigned>() );
        } catch ( const YAML::Exception&  e ) {
            m_zFailReason = QString::fromStdString( e.what() );
            cfgSetAutoload( 1 );
        }
    }
    else
    {
        cfgSetAutoload( 1 );
    }

    cfgSetLastOpenPath( QString::fromStdString( __yaml_GetString( m_cfg, "last open path" ) ) );
    cfgSetLastSavePath( QString::fromStdString( __yaml_GetString( m_cfg, "last save path" ) ) );
    cfgSetLastOpenFile( QString::fromStdString( __yaml_GetString( m_cfg, "last open file" ) ) );

    fp.close();

    return true;
}

bool  MainWindow::cfgWrite( const QString&  filename )
{
    std::string str;
    QFile 		fp( filename );
    QTextStream out( &fp );

    // открываем файл на запись
    if( !fp.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        m_zFailReason = "cannot create file";
        return false;
    }

    YAML::Node node;
    node[ "autoload" ] = cfgGetAutoload();
    node[ "last open path" ] = cfgGetLastOpenPath().toStdString();
    node[ "last save path" ] = cfgGetLastSavePath().toStdString();
    node[ "last open file" ] = cfgGetLastOpenFile().toStdString();

    // пробуем выгрузить ямл в строку
    try {
        str = YAML::Dump( node );
    } catch ( const YAML::Exception&  e ) {
        m_zFailReason = QString::fromStdString( e.what() );
        fp.close();
        return false;
    }

    // пишем строку в файл
    out.setCodec("UTF-8");
    out << QString::fromStdString( str );

    fp.close();

    return true;
}

//------------------------------------------------------------------------------

void  MainWindow::cfgSetAutoload( unsigned  load ) noexcept
{
    if( 0 == load )
        m_cfg_autoload = 0;
    else
        m_cfg_autoload = 1;
}

unsigned  MainWindow::cfgGetAutoload() noexcept
{
    return m_cfg_autoload;
}

void  MainWindow::cfgSetCurrentPath( const QString&  path ) noexcept
{
    m_cfg_current_path = QDir::toNativeSeparators( path );
}

QString  MainWindow::cfgGetCurrentPath() noexcept
{
    return m_cfg_current_path;
}

void  MainWindow::cfgSetLastOpenPath( const QString&  path ) noexcept
{
    if( 0 == path.length() )
        m_cfg_last_open_path = cfgGetCurrentPath();
    else
        m_cfg_last_open_path = QDir::toNativeSeparators( path );
}

void  MainWindow::cfgSetLastSavePath( const QString&  path ) noexcept
{
    if( 0 == path.length() )
        m_cfg_last_save_path = cfgGetCurrentPath();
    else
        m_cfg_last_save_path = QDir::toNativeSeparators( path );
}

void  MainWindow::cfgSetLastOpenFile( const QString&  file ) noexcept
{
    m_cfg_last_open_file = file;
}

QString  MainWindow::cfgGetLastOpenPath() noexcept
{
    return m_cfg_last_open_path;
}

QString  MainWindow::cfgGetLastSavePath() noexcept
{
    return m_cfg_last_save_path;
}

QString  MainWindow::cfgGetLastOpenFile() noexcept
{
    return m_cfg_last_open_file;
}

//------------------------------------------------------------------------------

void  MainWindow::closeEvent( QCloseEvent  *event )
{
    m_pGoods->GoodsDelete();

    m_config.reset();

    event->accept();
}

//------------------------------------------------------------------------------
