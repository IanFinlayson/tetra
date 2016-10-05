/* filerunner.h
 * code to run or debug a program */

#ifndef FILERUNNER_H
#define FILERUNNER_H

#include <sys/types.h>
#include <QElapsedTimer>
#include <QObject>
#include <QWaitCondition>

#include "tetra.h"

#include "mainwindow.h"

class MainWindow;
class FileRunner : public QObject, public VirtualConsole {
    Q_OBJECT
  public:
    FileRunner(MainWindow* mainWindow);

    /* functions for communicating with tetra I/O */
    String receiveStandardInput();
    void processStandardOutput(const String& text);
    void receiveInput(QString input);

    /* halt the running program in its tracks */
    void halt();

  signals:
    void finished();
    void output(QString text);
    void needInput();
    void errorSeen(QString message, int line);

  public slots:
    void runFile(bool debug);

  private:
    /* pointer to the main window */
    MainWindow* mainWindow;

    /* the file runner uses this to wait until input is ready */
    QWaitCondition inputReady;

    /* stores the input as supllied by the console */
    QString myInput;

    /* whether the input was interrupted by a stop */
    bool inputInterrupted;

    /* these keep track of the elapsed running time */
    QElapsedTimer programTimer;
};

#endif
