#ifndef FILERUNNER_H
#define FILERUNNER_H

#include <sys/types.h>
#include <QObject>
#include "backend.h"
#include "mainwindow.h"

class MainWindow;
class FileRunner : public QObject {
  Q_OBJECT
 public:
  FileRunner(MainWindow *mainWindow);

 private:
  MainWindow *mainWindow;

 signals:
  void finished();

 public slots:
  void runFile(bool debug);
};

#endif
