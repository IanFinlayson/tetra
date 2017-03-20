/* main.cpp
 * contains the application entry point */

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QObject>
#include <QStringList>
#include <QStyleFactory>

#include "editor.h"
#include "mainwindow.h"
#include "settingsmanager.h"

int main(int argc, char* argv[]) {
    /* set up the application */
    QApplication app(argc, argv);
    app.setOrganizationName("UMW Computer Science");
    app.setApplicationName("Tetra");

    /* for experimenting with different application styles
    QStringList styles = QStyleFactory::keys();
    for (int i = 0; i < styles.size(); i++) {
        std::cout << styles.at(i).toLocal8Bit().constData() << std::endl; 
    }
    app.setStyle("Breeze");
    */

    /* initialize the settings and the main window */
    SettingsManager::init();
    MainWindow w;
    w.show();

    /* try to open all the files we were passed as arguments */
    for (int i = 1; i < argc; i++) {
        w.doOpen(argv[i]);
    }

    /* start the main event loop */
    return app.exec();
}
