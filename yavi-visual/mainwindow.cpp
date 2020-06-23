#include <QtCore>
#include <QtGui>
#include <QSettings>

#include "mainwindow.h"
#include "func.h"
#include "dialog.h"

#define AppVendor   "Cobu"
#define AppName     "yavi"

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
    m_history.clear();

    QFrame  *frmBase = new QFrame();

    QHBoxLayout  *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    m_ptBtnNew = new QPushButton( tr( "New" ), this );
    connect( m_ptBtnNew, &QPushButton::clicked, this, &MainWindow::onBtnNew );
    hlayout->addWidget( m_ptBtnNew, 0, Qt::AlignLeft );

    // кнопка "загрузить yaml из файла"
    m_ptBtnOpen = new QPushButton( tr( "Open" ), this );
    connect( m_ptBtnOpen, &QPushButton::clicked, this, &MainWindow::onBtnOpen );
    hlayout->addWidget( m_ptBtnOpen, 0, Qt::AlignLeft );

    // кнопка "сохранить все в yaml"
    m_ptBtnSave = new QPushButton( tr( "Save" ), this );
    connect( m_ptBtnSave, &QPushButton::clicked, this, &MainWindow::onBtnSave );
    hlayout->addWidget( m_ptBtnSave, 0, Qt::AlignLeft );

    m_ptBtnUndo = new QPushButton( "↺", this );
    connect( m_ptBtnUndo, &QPushButton::clicked, this, &MainWindow::onBtnUndo );
    hlayout->addWidget( m_ptBtnUndo, 0, Qt::AlignRight );

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

void  MainWindow::onBtnNew()
{
    if( ! askSaveIfChanged() )
    {
        return;
    }

    bool ok;
    QString text = QInputDialog::getText( this, tr( "New category" ), tr( "Category name:"), QLineEdit::Normal, "", &ok);
    if( ok && !text.isEmpty() )
    {
        // clear all and make new value
        // похоже что уже есть открытый ямл
        if( !m_config.IsNull() )
        {
            m_pGoods->GoodsDelete();

            m_config.reset();
            m_history.clear();
        }

        m_config = YAML::Load( QString( "category: \n  - name: " +  text  + "\n\n" ).toStdString() );
        //m_config = YAML::Load( QString( "category: \n  - name: " +  text  + "\n\ntables: \n - id: table\n").toStdString() );
        m_pGoods->parse_yaml( m_config );
        addHistory( m_config );

        //onYamlChanged();

        setPrgTitleText();
    }
}

void  MainWindow::onBtnOpen()
{
    if( ! askSaveIfChanged() )
    {
        return;
    }

    // очищаем лэйбл с описанием ошибки
    m_ptLblNotice->clear();
//    m_ptLblNotice->setStyleSheet( "QLabel {background-color: transparent;}" );

    // каталог где мы находимся
    QDir *pDir = new QDir( cfgGetLastOpenFile() );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators( QFileDialog::getOpenFileName( this, tr( "Open file" ), dir, "YAML (*.yaml *.yml)" ) );

    if( !filename.isEmpty() )
    {
        // похоже что уже есть открытый ямл
        if( !m_config.IsNull() )
        {
            m_pGoods->GoodsDelete();

            m_config.reset();
            m_history.clear();
        }

        if( false == init( filename ) )
        {
            QString  zReport;
            zReport = tr( "Cannot open" ) + " "  + filename + " " + tr( "with result" ) + " -> " + m_zFailReason;

            m_ptLblNotice->setText( zReport );
//            m_ptLblNotice->setStyleSheet( "QLabel {background-color: red;}" );
        }
        else
        {
            // заголовок формы
            setPrgTitleText( filename );

            // запоминаем каталог
            cfgSetLastOpenPath( filename.section( QDir::separator(), 0, -2 ) );

            // запоминаем файл
            cfgSetLastOpenFile( filename );

            // обновляем ini
            writeSettings();
            //cfgRefresh();
        }
    }
    else
    {
        // не ввели имя файла

        //qDebug() << "Cannot open: no filename";
    }

    delete pDir;
}

void  MainWindow::onBtnSave()
{
    // формируем имя файла по умолчанию
    //QString deffilename = QString( "/test.yml" );

    // каталог где мы сохранялись крайний раз
    QDir *pDir = new QDir( cfgGetLastOpenFile() );

    // строка с именем каталога где мы сохранялись крайний раз
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators( QFileDialog::getSaveFileName( this, tr( "Save file" ), dir, "YAML (*.yml)" ) );

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
            // запоминаем файл
            cfgSetLastOpenFile( filename );

            // обновляем ini
            writeSettings();
            //cfgRefresh();
        }
    }
    else
    {
        // не ввели имя файла

//        qDebug() << "Cannot save: no filename";
    }

    delete pDir;
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
}

//------------------------------------------------------------------------------

bool  MainWindow::init( const QString&  filename )
{
    QFile  fp( filename );

    // а есть ли файл?
    if( !fp.exists() )
    {
        m_zFailReason = tr( "file is not exists" );
        return false;
    }

    // открываем файл только на чтение
    if( !fp.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        m_zFailReason = tr( "cannot open file" );
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

    addHistory( m_config );

    fp.close();

    // возможно ямл пустой
    if( m_pGoods->empty() )
    {
        m_zFailReason = tr( "file is empty" );
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
        m_zFailReason = tr( "cannot create file" );
        return false;
    }

    // пробуем выгрузить ямл в строку
    try {
        str = YAML::Dump( m_config );
    } catch ( const YAML::Exception&  e ) {
        // что-то пошло не так, а что смотрим в e.what()
        m_zFailReason = QString::fromStdString( e.what() );
        return false;
    }

    // пишем строку в файл
    out.setCodec("UTF-8");
    out << QString::fromStdString( str );

    fp.close();

    m_history.clear();
    addHistory( m_config );

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
    if( ! addHistory( m_config ) )
        return;

    // если признака что ямл изменился еще нет
    if( !m_bPrgTitleChanged )
    {
        // рисуем "*" перед именем файла
        setPrgTitleChanged( true );
    }

    // признак что ямл изменился
    m_bPrgTitleChanged = true;
}

void    MainWindow::keyPressEvent( QKeyEvent *event )
{
    if( event->matches( QKeySequence::Undo ) )
        undoHistory();
}

void         MainWindow::onBtnUndo()
{
    undoHistory();
}

bool          MainWindow::addHistory( const YAML::Node&  node )
{
    std::string     serialize = YAML::Dump( node );

    if( m_history.empty() )
    {
        showHistory();
        m_history.push_back( serialize );
        return true;
    }

    if( m_history.back() != serialize )
    {
        m_history.push_back( serialize );
        showHistory();
        return true;
    }

    showHistory();
    return false;
}

bool    MainWindow::undoHistory()
{
    if( m_history.size() <= 1 )
        return true;

    if( !m_config.IsNull() )
    {
        m_pGoods->GoodsDelete();

        m_config.reset();
    }

    m_history.pop_back();
    m_config = YAML::Load( m_history.back() );
    m_pGoods->parse_yaml( m_config );
    showHistory();

    if( 1 == m_history.size() )
    {
        setPrgTitleChanged( false );
        m_bPrgTitleChanged = false;
    }

    return true;
}

void    MainWindow::showHistory()
{
    int     depth = m_history.size()-1;

    if( depth <= 0 )
        statusBar()->clearMessage();
    else
        statusBar()->showMessage( tr( "Unsaved changes : " ) + QString::number( depth ) );
}

//------------------------------------------------------------------------------

void  MainWindow::actionAfterStart()
{
    //cfgReset();

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    cfgSetCurrentPath( dir );

    readSettings();
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

    delete pDir;
}

//------------------------------------------------------------------------------

void          MainWindow::readSettings()
{
     //QSettings  settings( QSettings::IniFormat, QSettings::UserScope, AppVendor, AppName );
     QSettings  settings( AppVendor, AppName );

     cfgSetAutoload( settings.value( "autoload", 1 ).toInt() );
     cfgSetLastOpenPath( settings.value( "last open path", "./" ).toString() );
     cfgSetLastSavePath( settings.value( "last save path", "./" ).toString() );
     cfgSetLastOpenFile( settings.value( "last open file", "./" ).toString() );
}

void          MainWindow::writeSettings()
{
     //QSettings  settings( QSettings::IniFormat, QSettings::UserScope, AppVendor, AppName );
     QSettings  settings( AppVendor, AppName );

     settings.setValue( "autoload", cfgGetAutoload() );
     settings.setValue( "last open path", cfgGetLastOpenPath() );
     settings.setValue( "last save path", cfgGetLastSavePath() );
     settings.setValue( "last open file", cfgGetLastOpenFile() );
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

bool          MainWindow::askSaveIfChanged()
{
    if( m_bPrgTitleChanged )
    {
        QMessageBox     msgBox;

        msgBox.setText( tr( "The document has been modified" ) );
        msgBox.setInformativeText( tr( "Do you want to save your changes?" ) );
        msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        msgBox.setDefaultButton( QMessageBox::Save );
        msgBox.button( QMessageBox::Save )->setText( tr( "Save" ) );
        msgBox.button( QMessageBox::Discard )->setText( tr( "Discard" ) );
        msgBox.button( QMessageBox::Cancel )->setText( tr( "Cancel" ) );

        int ret = msgBox.exec();

        switch (ret) 
        {
            case QMessageBox::Save:
                onBtnSave();
                break;

            case QMessageBox::Discard:
                break;

            case QMessageBox::Cancel:
                return false;
            default:
                return false;
        }
    }

    return true;
}

void  MainWindow::closeEvent( QCloseEvent  *event )
{
    if( ! askSaveIfChanged() )
    {
        event->ignore();
    }
    else
    {
        m_pGoods->GoodsDelete();

        m_config.reset();

        event->accept();
    }
}

//------------------------------------------------------------------------------

