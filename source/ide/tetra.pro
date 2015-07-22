# set the compiler to clang
QMAKE_CXX = clang++

# set options
CONFIG += qt debug thread warnoff

# specify icons
ICON = tetra.icns

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

# specify all the files we need
SOURCES += main.cpp\
    mainwindow.cpp \
    openappdialog.cpp \
    syntaxhighlighter.cpp \
    editor.cpp \
    filerunner.cpp \
    debugwindow.cpp

HEADERS  += \
    mainwindow.h \
    openappdialog.h \
    syntaxhighlighter.h \
    editor.h \
    filerunner.h \
    debugwindow.h \
    tetra.h

FORMS    += \
    mainwindow.ui \
    openappdialog.ui \
    openapp.ui

RESOURCES += \
    resources.qrc

