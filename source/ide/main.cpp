/* main.cpp
 * contains the application entry point */

#include <QApplication>
#include <QLabel>
#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QStyleFactory>

#include "settingsmanager.h"
#include "editor.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    /* set up the application */
    QApplication app(argc, argv);
    app.setOrganizationName("UMW Computer Science");
    app.setApplicationName("Tetra");

    /* list all available styles */
    //QStringList styles = QStyleFactory::keys();
    //for (int i = 0; i < styles.size(); i++) {
        //qDebug() << styles.at(i);
    //}

    /* set the style */
    //app.setStyle(QStyleFactory::create("Fusion"));

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
