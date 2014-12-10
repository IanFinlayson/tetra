#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "mainwindow.h"
#include "backend.hpp"
#include "commandObserver.h"
#include <sys/types.h>
#include <QObject>

class MainWindow;
class Debugger: public QObject
{
    Q_OBJECT
public:
    Debugger(MainWindow *mainWindow);

private:
    MainWindow *mainWindow;
    CommandObserver debugger = CommandObserver();

signals:
    void finished();

public slots:
    void debugFile();

};




#endif // DEBUGGER_H
