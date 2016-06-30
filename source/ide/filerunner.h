/* filerunner.h
 * code to run or debug a program */

#ifndef FILERUNNER_H
#define FILERUNNER_H

#include <sys/types.h>
#include <QObject>
#include "backend.h"
#include "mainwindow.h"

class MainWindow;
class FileRunner : public QObject, public VirtualConsole{
    Q_OBJECT
  public:
    FileRunner(MainWindow* mainWindow);

    /* functions for communicating with tetra I/O */
    std::string receiveStandardInput();
    void processStandardOutput(const std::string& text);

  private:
    MainWindow* mainWindow;

  signals:
    void finished();

  public slots:
    void runFile(bool debug);
};

#endif

