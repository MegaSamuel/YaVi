#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // задаем начальный размер формы так, чтобы она корректно отображалась на матрице 1366х768
    w.resize( 1280, 720 );

    // распологаем форму по центру основного экрана
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);

    w.show();

    return a.exec();
}

//------------------------------------------------------------------------------
