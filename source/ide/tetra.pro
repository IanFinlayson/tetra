# set the compiler to clang
QMAKE_CXX = clang++
QMAKE_LINK = clang++

# set compiler flags
QMAKE_CXXFLAGS += -std=c++11 -W -Wall -pedantic

# set options
CONFIG += qt debug thread

# set the QT modules we need
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# build an application
TARGET = tetraide
TEMPLATE = app

# include the interpreter path
INCLUDEPATH += $$PWD/../interpreter
DEPENDPATH += $$PWD/../interpreter

# link to the tetra interpreter library, and the gc library
PRE_TARGETDEPS += $$PWD/../interpreter/libtetra.a
LIBS += -L$$PWD/../interpreter/ -ltetra -lgc

# add target to reformat code
format.commands = clang-format -i *.cpp *.h
QMAKE_EXTRA_TARGETS += format

# specify all the files we need
SOURCES += main.cpp\
    mainwindow.cpp \
    syntaxhighlighter.cpp \
    editor.cpp \
    console.cpp \
    settingsdialog.cpp \
    settingsmanager.cpp \
    searchbox.cpp \
    replacedialog.cpp \
    filerunner.cpp

HEADERS  += \
    mainwindow.h \
    syntaxhighlighter.h \
    editor.h \
    console.h \
    settingsdialog.h \
    settingsmanager.h \
    searchbox.h \
    replacedialog.h \
    filerunner.h

FORMS    += \
    mainwindow.ui \
    about.ui \
    settings.ui \
    replace.ui

RESOURCES += \
    resources.qrc

# add a post compile step to copy the file
QMAKE_POST_LINK  = "cp tetraide ../../bin && echo 'Finished Building IDE'"

