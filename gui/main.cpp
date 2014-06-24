#include "mainwindow.h"
#include "openappdialog.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("UMW Computer Science");
    app.setApplicationName("Tetra");
    MainWindow w;
    w.showMaximized();

    OpenAppDialog openappdialog;
    openappdialog.setMainWindow(&w);
    openappdialog.show();

    return app.exec();
}
