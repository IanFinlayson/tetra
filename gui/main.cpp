 #include "mainwindow.h"
#include "openappdialog.h"
#include "editor.h"
#include <QApplication>
#include <QObject>
#include <QLabel>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("UMW Computer Science");
    app.setApplicationName("Tetra");

    MainWindow w;
    w.showMaximized();


    //QGridLayout newLayout;
    Editor* newEditor = w.getEditor();
    //newLayout.addWidget(newEditor,0,0,1,1,0);
    QGridLayout* thisLayout = w.getGridLayout();
    //for(int i = 0; i < 2; i++){
      //  thisLayout->addWidget(new Editor, 1, 1, 1, 1, 0);
    //}
    //thisLayout->addWidget(newEditor, 2, 2, 0, 1,0);
    OpenAppDialog openappdialog;
    openappdialog.setMainWindow(&w);
    openappdialog.show();




    return app.exec();
}
