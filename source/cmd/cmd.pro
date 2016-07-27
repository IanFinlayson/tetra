# cmd.pro
# qmake project file for the command line interpreter

# set the compiler to clang
QMAKE_CXX = clang++
QMAKE_LINK = clang++

# set compiler flags
QMAKE_CXXFLAGS += -std=c++11 -W -Wall -pedantic

# include the lib path
INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

# link to the tetra interpreter library, and the gc library
PRE_TARGETDEPS += $$PWD/../lib/libtetra.a
LIBS += -L$$PWD/../lib/ -ltetra -lgc

TEMPLATE = app
SOURCES = commandConsole.cpp \
	commandObserver.cpp \
	main.cpp
LIBS += ../lib/libtetra.a
TARGET = ../../bin/tetra

