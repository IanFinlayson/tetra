# lib.pro
# qmake project file for the interpreter library

# set compiler flags
QMAKE_CXXFLAGS += -std=c++11 -W -Wall -pedantic -ggdb3
QMAKE_CXXFLAGS += -O0
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O3

# stop Qt from writing messages in the terminal
DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

# build as a static library
TEMPLATE = lib
CONFIG += staticlib debug
TARGET = tetra

# specify the files used
HEADERS = $$files(*.h)
HEADERS -= Parser.h

SOURCES = $$files(*.cpp)
SOURCES -= Parser.cpp

# all of this is to make bison work correctly
BISONSOURCES = Parser.y
OTHER_FILES += $$BISONSOURCES
bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison -d --defines=${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += bisonsource
bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_FILE_BASE}.h
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = Bison Headers ${QMAKE_FILE_IN}
bisonheader.CONFIG += target_predeps no_link
QMAKE_EXTRA_COMPILERS += bisonheader

