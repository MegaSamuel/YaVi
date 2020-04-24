#-------------------------------------------------
#
# Project created by QtCreator 2020-04-22T22:22:22
#
#-------------------------------------------------

QT       -= gui

TARGET = yaml-cpp
TEMPLATE = lib
CONFIG += staticlib

TOPDIR = $$PWD/..
include( $$TOPDIR/common.pri )

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

CONFIG(debug,debug|release) {
        DEFINES += __DEBUG__
}

CFLAGS += -fPIC
CXXFLAGS += -fPIC

yaml_sources =  \
    src/contrib/graphbuilder.cpp \
    src/contrib/graphbuilderadapter.cpp \
    src/binary.cpp \
    src/convert.cpp \
    src/directives.cpp \
    src/emit.cpp \
    src/emitfromevents.cpp \
    src/emitter.cpp \
    src/emitterstate.cpp \
    src/emitterutils.cpp \
    src/exceptions.cpp \
    src/exp.cpp \
    src/memory.cpp \
    src/node.cpp \
    src/node_data.cpp \
    src/nodebuilder.cpp \
    src/nodeevents.cpp \
    src/null.cpp \
    src/ostream_wrapper.cpp \
    src/parse.cpp \
    src/parser.cpp \
    src/regex_yaml.cpp \
    src/scanner.cpp \
    src/scanscalar.cpp \
    src/scantag.cpp \
    src/scantoken.cpp \
    src/simplekey.cpp \
    src/singledocparser.cpp \
    src/stream.cpp \
    src/tag.cpp

yaml_headers = \
    src/contrib/graphbuilderadapter.h \
    src/collectionstack.h \
    src/directives.h \
    src/emitterstate.h \
    src/emitterutils.h \
    src/exp.h \
    src/indentation.h \
    src/nodebuilder.h \
    src/nodeevents.h \
    src/ptr_vector.h \
    src/regex_yaml.h \
    src/regeximpl.h \
    src/scanner.h \
    src/scanscalar.h \
    src/scantag.h \
    src/setting.h \
    src/singledocparser.h \
    src/stream.h \
    src/streamcharsource.h \
    src/stringsource.h \
    src/tag.h \
    src/token.h

yaml_include = \
    include/yaml-cpp/contrib/anchordict.h \
    include/yaml-cpp/contrib/graphbuilder.h \
    include/yaml-cpp/node/detail/bool_type.h \
    include/yaml-cpp/node/detail/impl.h \
    include/yaml-cpp/node/detail/iterator.h \
    include/yaml-cpp/node/detail/iterator_fwd.h \
    include/yaml-cpp/node/detail/memory.h \
    include/yaml-cpp/node/detail/node.h \
    include/yaml-cpp/node/detail/node_data.h \
    include/yaml-cpp/node/detail/node_iterator.h \
    include/yaml-cpp/node/detail/node_ref.h \
    include/yaml-cpp/node/convert.h \
    include/yaml-cpp/node/emit.h \
    include/yaml-cpp/node/impl.h \
    include/yaml-cpp/node/iterator.h \
    include/yaml-cpp/node/node.h \
    include/yaml-cpp/node/parse.h \
    include/yaml-cpp/node/ptr.h \
    include/yaml-cpp/node/type.h \
    include/yaml-cpp/anchor.h \
    include/yaml-cpp/binary.h \
    include/yaml-cpp/dll.h \
    include/yaml-cpp/emitfromevents.h \
    include/yaml-cpp/emitter.h \
    include/yaml-cpp/emitterdef.h \
    include/yaml-cpp/emittermanip.h \
    include/yaml-cpp/emitterstyle.h \
    include/yaml-cpp/eventhandler.h \
    include/yaml-cpp/exceptions.h \
    include/yaml-cpp/mark.h \
    include/yaml-cpp/noncopyable.h \
    include/yaml-cpp/null.h \
    include/yaml-cpp/ostream_wrapper.h \
    include/yaml-cpp/parser.h \
    include/yaml-cpp/stlemitter.h \
    include/yaml-cpp/traits.h \
    include/yaml-cpp/yaml.h \

SOURCES += $$yaml_sources

HEADERS += \
        $$yaml_headers \
        $$yaml_include

INCLUDEPATH += $$PWD/include

target_lib = $(DESTDIR_TARGET)
target_lib_path = $$PWD/../target/$$TEMPLATE
target_headers = include/yaml-cpp
target_headers_path = $$PWD/../include

unix {
    target.path += $$target_lib_path
    INSTALLS += target
    headers.path = $$target_headers_path
    headers.files += $$target_headers
    INSTALLS += headers
    QMAKE_POST_LINK = make install
}

win32 {
    chkfile = $$target_lib_path/.gitignore
    chkfile ~= s,/,\\,g
    target_lib_path ~= s,/,\\,g
    exists( $$chkfile ) {
        QMAKE_POST_LINK += $$quote(cmd /c $(COPY_FILE) $$target_lib $$target_lib_path $$escape_expand(\n\t))
    }

    target_headers = $$PWD/$$target_headers
    target_headers ~= s,/,\\,g
    target_headers_path ~= s,/,\\,g
    QMAKE_POST_LINK += $$quote(cmd /c $(COPY_DIR) $$target_headers $$target_headers_path\yaml-cpp $$escape_expand(\n\t))
}
