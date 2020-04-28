#include "mainwindow.h"
#include "work.h"

using namespace cobu;

//------------------------------------------------------------------------------

QWidget* MainWindow::createStuff( QWidget  *parent )
{
    QFrame  *frmBase = new QFrame( parent );

    QHBoxLayout  *hlayout = new QHBoxLayout;

    // кнопка "загрузить yaml из файла"
    m_ptBtnOpen = new QPushButton( "Open", this );
    connect( m_ptBtnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpen()) );
    hlayout->addWidget( m_ptBtnOpen, 0, Qt::AlignLeft );

    // кнопка "сохранить все в yaml"
    m_ptBtnSave = new QPushButton( "Save", this );
    connect( m_ptBtnSave, SIGNAL(clicked()), this, SLOT(onBtnSave()) );
    hlayout->addWidget( m_ptBtnSave, 0, Qt::AlignLeft );

    // лэйбл
    m_ptLblNotice = new QLabel( this, Q_NULLPTR );
    //By default, the contents of the label are left-aligned and vertically-centered.
    //m_ptLblNotice->setAlignment( Qt::AlignLeft );
    m_ptLblNotice->setFrameStyle( QFrame::NoFrame );
    hlayout->addWidget( m_ptLblNotice, 0, Qt::AlignLeft );

    // пружинка
    hlayout->addStretch( 0 );

    QVBoxLayout  *vlayout = new QVBoxLayout;

    // добавляем горизонтальный layout в вертикальный layout
    vlayout->addLayout( hlayout );

    // добавляем то где будет дерево ямла в вертикальный layout
//    vlayout->addLayout( hlayout );
    // пружинка
    vlayout->addStretch( 0 );

    // добавляем вертикальный layout в frame
    frmBase->setLayout( vlayout );
//    frmBase->setFrameShape( QFrame::Box );

    return frmBase;
}

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // заголовок формы
    this->setWindowTitle("YAML Visualizer");

    // иконка формы
    this->setWindowIcon( QIcon( ":/favicon.ico" ) );

    // нополнение формы
    auto wgtCentral = createStuff( this );

    // центральный элеиент
    setCentralWidget( wgtCentral );

//    m_pWork = Q_NULLPTR;
    m_pWork = new Work();
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
//    QString deffilename = QString( "/pattern%1x%2.yml" ).arg(m_uRow).arg(m_uColumn);
    QString deffilename = QString( "/test.yml" );

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() + deffilename );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName( this, "Save file", dir, "YAML (*.yaml *.yml)" );

    if( !filename.isEmpty() )
    {
        if( false == m_pWork->fini( filename ) )
        {
            QString  zReport;
            zReport = "Cannot open " + filename + " with result -> " + m_pWork->zFailReason;

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

