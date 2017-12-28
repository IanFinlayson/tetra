# cmd.pro
# qmake project file for the command line interpreter

# set compiler flags
QMAKE_CXXFLAGS += -std=c++11 -W -Wall -pedantic -ggdb3
QMAKE_CXXFLAGS += -O0
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O3

# stop Qt from writing messages in the terminal
DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

# include the lib path
INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

# link to the tetra interpreter library
PRE_TARGETDEPS += $$PWD/../lib/libtetra.a
LIBS += -L$$PWD/../lib/ -ltetra

TEMPLATE = app
CONFIG += debug
SOURCES = $$files(*.cpp)

LIBS += ../lib/libtetra.a
TARGET = ../../bin/tetra

