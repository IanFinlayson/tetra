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
PRE_TARGETDEPS += $$PWD/../interpreter/libtetra.a
LIBS += -L$$PWD/../interpreter/ -ltetra

# add target to reformat code
format.commands = clang-format -i -style=Google *.cpp *.h
QMAKE_EXTRA_TARGETS += format

# specify all the files we need
SOURCES += main.cpp\
    mainwindow.cpp \
    syntaxhighlighter.cpp \
    editor.cpp \
    filerunner.cpp \
    debugwindow.cpp

HEADERS  += \
    mainwindow.h \
    syntaxhighlighter.h \
    editor.h \
    filerunner.h \
    debugwindow.h \
    tetra.h

FORMS    += \
    mainwindow.ui \
    openapp.ui

RESOURCES += \
    resources.qrc

# add a post compile step to copy the file
QMAKE_POST_LINK  = "cp tetraide ../../bin && echo 'Finished Building IDE'"

