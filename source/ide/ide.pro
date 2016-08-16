# ide.pro
# qmake project file for the IDE

# set the compiler to clang
QMAKE_CXX = clang++
QMAKE_LINK = clang++

# set compiler flags
QMAKE_CXXFLAGS += -std=c++11 -W -Wall -pedantic

# set options
CONFIG += qt debug thread

# set the QT modules we need
QT += core gui widgets printsupport

# build an application
TARGET = ../../bin/tetraide
TEMPLATE = app

# include the lib path
INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

# link to the tetra library
PRE_TARGETDEPS += $$PWD/../lib/libtetra.a
LIBS += -L$$PWD/../lib/ -ltetra

# specify all the files we need
SOURCES += $$files(*.cpp)
HEADERS += $$files(*.h)
FORMS += $$files(*.ui)
RESOURCES += $$files(*.qrc)

