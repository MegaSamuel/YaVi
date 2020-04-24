#DEFINES += 

QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS += -Wshadow -Wformat=2 -Wfloat-equal -Wlogical-op -Wcast-qual -Wcast-align
QMAKE_CXXFLAGS += -D_FORTIFY_SOURCE=2
QMAKE_CXXFLAGS += -fno-sanitize-recover
