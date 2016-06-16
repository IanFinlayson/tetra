# set the compiler to clang
QMAKE_CXX = clang++
QMAKE_LINK = clang++

# set options
CONFIG += qt debug thread warnoff

# TODO specify icons

# set the QT modules we need
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# build an application
TARGET = tetraide
TEMPLATE = app

# include the interpreter path
INCLUDEPATH += $$PWD/../interpreter
DEPENDPATH += $$PWD/../interpreter

# link to the tetra interpreter library
# temporarily use the one in this dir...
#PRE_TARGETDEPS += $$PWD/../interpreter/libtetra.a
PRE_TARGETDEPS += $$PWD/libtetra.a
LIBS += -L$$PWD/../interpreter/ -ltetra

# add target to reformat code
format.commands = clang-format -i *.cpp *.h
QMAKE_EXTRA_TARGETS += format

# specify all the files we need
SOURCES += main.cpp\
    mainwindow.cpp \
    syntaxhighlighter.cpp \
    editor.cpp \
    console.cpp \
    filerunner.cpp

HEADERS  += \
    mainwindow.h \
    syntaxhighlighter.h \
    editor.h \
    console.h \
    filerunner.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# add a post compile step to copy the file
QMAKE_POST_LINK  = "cp tetraide ../../bin && echo 'Finished Building IDE'"

