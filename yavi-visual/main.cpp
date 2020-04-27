#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

using namespace cobu;

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // задаем начальный размер формы так, чтобы она корректно отображалась на матрице 1366х768
//    w.resize( 1280, 720 );
    w.resize( 800, 600 );

    // распологаем форму по центру основного экрана
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);

/*
    // определяем шрифт
    QFont newFont( "Tahoma", 8, QFont::Thin, false );
    // применяем ко всему приложению
    QApplication::setFont(newFont);
*/

    w.show();

    return a.exec();
}

//------------------------------------------------------------------------------
