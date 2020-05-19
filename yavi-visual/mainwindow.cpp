#include "mainwindow.h"
//#include "work.h"

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_fBeginTime = cpu_time();

    // создаем и запускаем основной таймер
    m_uTimerCounter = 0;
    m_ptTimer = new QTimer( this );
    connect( m_ptTimer, SIGNAL(timeout()), this, SLOT(onTimerWork()) );
    m_ptTimer->start(1000); // 1 Hz

    // заголовок формы
    this->setWindowTitle("YAML Visualizer");

    // иконка формы
    this->setWindowIcon( QIcon( ":/favicon.ico" ) );

    QFrame  *frmBase = new QFrame();

    QHBoxLayout  *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    // кнопка "загрузить yaml из файла"
    m_ptBtnOpen = new QPushButton( "Open" );
    connect( m_ptBtnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpen()) );
    hlayout->addWidget( m_ptBtnOpen, 0, Qt::AlignLeft );

    // кнопка "сохранить все в yaml"
    m_ptBtnSave = new QPushButton( "Save" );
    connect( m_ptBtnSave, SIGNAL(clicked()), this, SLOT(onBtnSave()) );
    hlayout->addWidget( m_ptBtnSave, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblNotice = new QLabel( Q_NULLPTR );
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
}

MainWindow::~MainWindow()
{

}

//------------------------------------------------------------------------------

void 	MainWindow::onBtnOpen()
{
//    qDebug() << "Open button";

    // очищаем лэйбл с описанием ошибки
    m_ptLblNotice->clear();
//    m_ptLblNotice->setStyleSheet( "QLabel {background-color: transparent;}" );

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getOpenFileName( this, "Open file", dir, "YAML (*.yaml *.yml)" );

    if( !filename.isEmpty() )
    {
        // похоже что уже есть открытый ямл
        if( !m_config.IsNull() )
        {
            qDebug() << "m_config does not empty!";

            m_pGoods->GoodsDelete();
        }

        if( false == init( filename ) )
        {
            QString  zReport;
            zReport = "Cannot open " + filename + " with result -> " + zFailReason;

//            qDebug() << zReport;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
//            qDebug() << "Open file" << filename;
        }
    }
    else
    {
        // долбоящер не ввел имя файла

        qDebug() << "Cannot open: no filename";
    }
}

void 	MainWindow::onBtnSave()
{
//    qDebug() << "Save button";

    // формируем имя файла по умолчанию
    QString deffilename = QString( "/test.yml" );

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() + deffilename );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName( this, "Save file", dir, "YAML (*.yml)" );

    if( !filename.isEmpty() )
    {
        if( false == fini( filename ) )
        {
            QString  zReport;
            zReport = "Cannot write " + filename + " with result -> " + zFailReason;

//            qDebug() << zReport;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
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
        zFailReason = "file is not exists";
        return false;
    }

    // открываем файл только на чтение
    if( !fp.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        zFailReason = "cannot open file";
        return false;
    }

    QTextStream  in( &fp );

    in.setCodec("UTF-8");

    // пробуем читать ямл
    try {
        m_config = YAML::Load( in.readAll().toStdString() );
    } catch ( const YAML::Exception&  e ) {
        // что-то пошло не так, а что смотрим в e.what()
        zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // разбираем ямл
    m_pGoods->parse_yaml( m_config );

    fp.close();

    // возможно ямл пустой
    if( m_pGoods->empty() )
    {
        zFailReason = "file is empty";
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
        zFailReason = "cannot create file";
        return false;
    }

/*
    YAML::Node node1;
    node1["name"] = "node 1";
    node1["type"] = 0;
    node1["prostoslovo11"] = "prostoznachenie11";
    node1["prostoslovo12"] = "prostoznachenie12";
    node1["prostoslovo13"] = "prostoznachenie13";
    node1["prostoslovo14"] = "prostoznachenie14";

    YAML::Node node2;
    node2["name"] = "node 2";
    node2["type"] = 0;
    node2["prostoslovo21"] = "prostoznachenie21";
    node2["prostoslovo22"] = "prostoznachenie22";
    node2["prostoslovo23"] = "prostoznachenie23";
    node2["prostoslovo24"] = "prostoznachenie24";

    YAML::Node node3;
    node3["name"] = "node 3";
    node3["type"] = 0;
    node3["prostoslovo31"] = "prostoznachenie31";
    node3["prostoslovo32"] = "prostoznachenie32";
    node3["prostoslovo33"] = "prostoznachenie33";
    node3["prostoslovo34"] = "prostoznachenie34";

    YAML::Node node_name;
    node_name[ "name" ] = "NameStr";
    node_name[ "parameters" ].push_back(node1);
    node_name[ "parameters" ].push_back(node2);
    node_name[ "parameters" ].push_back(node3);

    YAML::Node node_main;
    node_main["category"].push_back( node_name );
*/
/*
    YAML::Node param = m_config["category"]["parameters"];

    for( size_t i = 0; i < param.size(); i++ )
    {
        qDebug() << i;
//        if( param[i]["name"].as<string>() == id) {
//            param.remove(i);
//            break;
//        }
    }
*/

    // пробуем выгрузить ямл в строку
    try {
        str = YAML::Dump( m_config );
//        str = YAML::Dump( node_main );
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
