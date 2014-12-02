#include "mainwindow.h"

#include "filerunner.h"

FileRunner::FileRunner(MainWindow* mainWindow){
    this->mainWindow = mainWindow;
}

void FileRunner::runFile(){
    Node* newNode;
    try{
        newNode = parseFile(mainWindow->getOpenFile().toStdString());
        mainWindow->setMainValue(interpret(newNode));
        mainWindow->setBuildSuccessful(true);
        mainWindow->printMainValue();
    }
    catch (RuntimeError e){

    }
    catch (SystemError e){

    }
    catch (Error e){
        mainWindow->printError(e);
    }
}

