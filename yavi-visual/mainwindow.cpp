#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // form caption
    this->setWindowTitle("YAML Visualizer");

    // form icon
    this->setWindowIcon( QIcon( ":/favicon.ico" ) );
}

MainWindow::~MainWindow()
{

}
