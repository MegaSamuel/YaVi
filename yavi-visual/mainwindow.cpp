#include "mainwindow.h"
#include "work.h"

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
}

MainWindow::~MainWindow()
{

}

void 	MainWindow::onBtnOpen()
{
    qDebug() << "Open button";

    // собственно файл
    QFile file;

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getOpenFileName( this, "Open file", dir, "YAML (*.yml)" );

    m_pWork = new Work( filename );

    qDebug() << "Open file" << filename;
}

void 	MainWindow::onBtnSave()
{
    qDebug() << "Save button";

    // собственно файл
    QFile file;

    // формируем имя файла по умолчанию
//    QString deffilename = QString( "/pattern%1x%2.yml" ).arg(m_uRow).arg(m_uColumn);
    QString deffilename = QString( "/test.yml" );

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() + deffilename );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName( this, "Save file", dir, "YAML (*.yml)" );

    if( filename != "" )
    {
        file.setFileName( filename );

        if( !file.open(QIODevice::WriteOnly) )
        {
            qDebug() << "cannot open file" << filename;

            return;
        }
        else
        {
            qDebug() << "write to file" << filename;

            file.resize(0);

//            BitMapCreate( &file );

            file.close();
        }
    }
    else
    {
        // долбоящер не ввел имя файла

        qDebug() << "no filename";
    }
}
