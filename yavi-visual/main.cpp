#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // set form to center of primary screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);

    // set fixed size for form
    w.setFixedSize( 800, 600 );

    // choose font
    QFont newFont( "Tahoma", 8, QFont::Thin, false );
    // use it for application
    QApplication::setFont(newFont);

    // shazam!
    w.show();

    return a.exec();
}
