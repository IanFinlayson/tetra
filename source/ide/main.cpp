/* main.cpp
 * contains the application entry point */

#include <QApplication>
#include <QLabel>
#include <QObject>

#include "settingsmanager.h"
#include "editor.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("UMW Computer Science");
    app.setApplicationName("Tetra");

    SettingsManager::init();
    MainWindow w;
    w.show();

    for (int i = 1; i < argc; i++) {
        w.doOpen(argv[i]);
    }

    return app.exec();
}
