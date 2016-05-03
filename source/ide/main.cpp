#include <QApplication>
#include <QLabel>
#include <QObject>
#include "editor.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setOrganizationName("UMW Computer Science");
  app.setApplicationName("Tetra");

  MainWindow w;
  w.showMaximized();
  w.startProject();

  return app.exec();
}
