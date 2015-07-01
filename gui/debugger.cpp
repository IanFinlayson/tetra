#include "mainwindow.h"
#include "debugger.h"
#include <QDebug>

Debugger::Debugger(MainWindow* mainWindow){
    this->mainWindow = mainWindow;
}

void Debugger::debugFile(){
    qDebug() << "HERE";
    Node* newNode;
    Console console(this->mainWindow);
    //TetraEnvironment::initialize(console);
    qDebug() << "1";

    TetraEnvironment::setObserver(debugger);
    qDebug() << "1";

    try{
        qDebug() << "1";
         qDebug() << "2";
        //TetraEnvironment::getObserver().step_E();
        qDebug() << "3";
        interpret(newNode);
        qDebug() << "4";
        //mainWindow->setBuildSuccessful(true);
        //mainWindow->printMainValue();
    }
    catch (RuntimeError e){

    }
    catch (SystemError e){

    }
    catch (Error e){
        mainWindow->printError(e);
    }
    emit finished();
}
