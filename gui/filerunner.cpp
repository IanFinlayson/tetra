#include "filerunner.h"
#include "commandObserver.h"
#include <QDebug>

FileRunner::FileRunner(MainWindow* mainWindow){
    this->mainWindow = mainWindow;
}

void FileRunner::runFile(){
    Node* newNode;
    Console console(this->mainWindow);
    qDebug() << "1";
    CommandObserver debugger = CommandObserver();
    qDebug() << "2";

    TetraEnvironment::setObserver(debugger);
    qDebug() << "3";

    TetraEnvironment::initialize(console);
    qDebug() << "4";

    try{
        newNode = parseFile(mainWindow->getOpenFile().toStdString());
        qDebug() << "5";

        mainWindow->setMainValue(interpret(newNode));
        qDebug() << "6";

        mainWindow->setBuildSuccessful(true);
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

