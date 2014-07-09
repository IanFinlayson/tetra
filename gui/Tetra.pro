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


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../frontend/release/ -lfrontend
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../frontend/debug/ -lfrontend
else:unix: LIBS += -L$$PWD/../frontend/ -lfrontend

INCLUDEPATH += $$PWD/../frontend
DEPENDPATH += $$PWD/../frontend

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../frontend/release/libfrontend.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../frontend/debug/libfrontend.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../frontend/release/frontend.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../frontend/debug/frontend.lib
else:unix: PRE_TARGETDEPS += $$PWD/../frontend/libfrontend.a
