#include "mainwindow.h"
#include "editor.h"
#include <QApplication>
#include <QObject>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("UMW Computer Science");
    app.setApplicationName("Tetra");

    MainWindow w;
    w.showMaximized();
    w.startProject( );

    return app.exec();
}
