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

# link to the tetra library, and the gc library
PRE_TARGETDEPS += $$PWD/../lib/libtetra.a
LIBS += -L$$PWD/../lib/ -ltetra -lgc

# specify all the files we need
SOURCES += main.cpp\
    mainwindow.cpp \
    syntaxhighlighter.cpp \
    editor.cpp \
    console.cpp \
    settingsdialog.cpp \
    settingsmanager.cpp \
    ideCommandObserver.cpp \
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
    ideCommandObserver.h \
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

