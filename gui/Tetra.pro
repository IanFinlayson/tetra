#-------------------------------------------------
#
# Project created by QtCreator 2014-05-20T12:01:09
#
#-------------------------------------------------

ICON = tetra.icns
QMAKE_INFO_PLIST = Info.plist
OTHER_FILES += Info.plist

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Tetra
TEMPLATE = app


SOURCES += main.cpp\
    quitdialog.cpp \
    mainwindow.cpp \
    openappdialog.cpp \
    syntaxhighlighter.cpp \
    editor.cpp

HEADERS  += \
    quitdialog.h \
    mainwindow.h \
    openappdialog.h \
    syntaxhighlighter.h \
    editor.h

FORMS    += \
    quitdialog.ui \
    mainwindow.ui \
    openappdialog.ui

RESOURCES += \
    AppResources.qrc
