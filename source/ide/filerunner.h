/* filerunner.h
 * code to run or debug a program */

#ifndef FILERUNNER_H
#define FILERUNNER_H

#include <sys/types.h>
#include <QElapsedTimer>
#include <QObject>
#include <QWaitCondition>
#include "backend.h"
#include "mainwindow.h"

class MainWindow;
class FileRunner : public QObject, public VirtualConsole {
    Q_OBJECT
  public:
    FileRunner(MainWindow* mainWindow);

    /* functions for communicating with tetra I/O */
    tstring receiveStandardInput();
    void processStandardOutput(const tstring& text);
    void receiveInput(QString input);

  signals:
    void finished();
    void output(QString text);
    void needInput();

  public slots:
    void runFile(bool debug);

  private:
    /* pointer to the main window */
    MainWindow* mainWindow;

    /* the file runner uses this to wait until input is ready */
    QWaitCondition inputReady;

    /* stores the input as supllied by the console */
    QString myInput;

    /* these keep track of the elapsed running time and time waiting for input */
    QElapsedTimer programTimer;
    qint64 inputTimer;
};

#endif

