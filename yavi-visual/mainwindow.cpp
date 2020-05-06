#include "mainwindow.h"
#include "work.h"

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // заголовок формы
    this->setWindowTitle("YAML Visualizer");

    // иконка формы
    this->setWindowIcon( QIcon( ":/favicon.ico" ) );

    QFrame  *frmBase = new QFrame();

    QHBoxLayout  *hlayout = new QHBoxLayout;
    hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
//    hlayout->setMargin(10);
//    hlayout->setSpacing(10);

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

    // пружинка
    //hlayout->addStretch( 0 );

    QVBoxLayout  *vlayout = new QVBoxLayout;
    vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
//    vlayout->setMargin(10);
//    vlayout->setSpacing(10);

    // добавляем горизонтальный layout в вертикальный layout
    vlayout->addLayout( hlayout );

    // добавляем то где будет дерево ямла в вертикальный layout
    // ->
    QScrollArea *scroll = new QScrollArea;
//    scroll->setMinimumWidth(256);
//    scroll->setMinimumHeight(256);
    scroll->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_pWork = new TWork();
//    vlayout->addWidget( m_pWork );

    scroll->setWidget( m_pWork );

    vlayout->addWidget( scroll );
    // <-

    // пружинка
//    vlayout->addStretch( 0 );

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
        if( false == m_pWork->init( filename ) )
        {
            QString  zReport;
            zReport = "Cannot open " + filename + " with result -> " + m_pWork->zFailReason;

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
        if( false == m_pWork->fini( filename ) )
        {
            QString  zReport;
            zReport = "Cannot write " + filename + " with result -> " + m_pWork->zFailReason;

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
