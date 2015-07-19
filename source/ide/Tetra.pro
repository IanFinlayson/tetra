
ICON = tetra.icns
QMAKE_INFO_PLIST = Info.plist
OTHER_FILES += Info.plist

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Tetra
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    openappdialog.cpp \
    syntaxhighlighter.cpp \
    editor.cpp \
    filerunner.cpp \
    openapp.cpp \
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
    AppResources.qrc

INCLUDEPATH += $$PWD/../interpreter
DEPENDPATH += $$PWD/../interpreter

unix:!macx: PRE_TARGETDEPS += $$PWD/../interpreter/libtetra.a
unix:!macx: LIBS += -L$$PWD/../interpreter/ -ltetra


