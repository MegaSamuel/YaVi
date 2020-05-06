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
    qDebug() << "Open button";

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
        if( false == init( filename ) )
        {
            QString  zReport;
            zReport = "Cannot open " + filename + " with result -> " + zFailReason;

            qDebug() << zReport;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
            qDebug() << "Open file" << filename;
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
    qDebug() << "Save button";

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

            qDebug() << zReport;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
            qDebug() << "Save file" << filename;
        }
    }
    else
    {
        // долбоящер не ввел имя файла

        qDebug() << "Cannot save: no filename";
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

bool  MainWindow::fini( const QString&  filename )
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
