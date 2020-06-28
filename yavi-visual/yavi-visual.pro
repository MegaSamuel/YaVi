#-------------------------------------------------
#
# Project created by QtCreator 2020-04-22T22:22:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yavi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

include(../common.pri)

INCLUDEPATH += $$PWD/../include

LIBS += -L$$PWD/../target/lib -lyaml-cpp

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    goods.cpp \
    table.cpp \
    category.cpp \
    param.cpp \
    dialog.cpp \
    table_dialog.cpp \
    table_entry.cpp \
    table_entryvalue.cpp

HEADERS += \
    mainwindow.h \
    goods.h \
    table.h \
    category.h \
    func.h \
    param.h \
    dialog.h \
    values.h \
    table_entry.h \
    table_entryvalue.h \
    table_dialog.h

win32: RC_ICONS = $$PWD/favicon.ico

RESOURCES += \
    yavi-visual.qrc

unix {
    target.path = $$PWD/../target/bin
    INSTALLS += target
    QMAKE_POST_LINK = make install
}

win32 {
    target_path = $$PWD/../target/bin
    target_path ~= s,/,\\,g
    chkfile = $$target_path/.gitignore
    chkfile ~= s,/,\\,g
    exists( $$chkfile ) {
        QMAKE_POST_LINK += $$quote(cmd /c $(COPY_FILE) $(DESTDIR_TARGET) $$target_path $$escape_expand(\n\t))
    }
}
