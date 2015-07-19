#ifndef FILERUNNER_H
#define FILERUNNER_H

#include "mainwindow.h"
#include "backend.h"
#include <sys/types.h>
#include <QObject>

class MainWindow;
class FileRunner: public QObject
{
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

