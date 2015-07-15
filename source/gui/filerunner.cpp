#include "filerunner.h"
#include "ideCommandObserver.h"
#include <QDebug>

int interpret(Node*, std::string[], int);

FileRunner::FileRunner(MainWindow* mainWindow){
    this->mainWindow = mainWindow;

}

//run or debugs file
void FileRunner::runFile(bool debug){
    ConsoleArray consoleArray;
    Console console(mainWindow);
    consoleArray.registerConsole(console);
    Node* newNode;
    IDECommandObserver debugger = IDECommandObserver();
    TetraEnvironment::setObserver(debugger);
    TetraEnvironment::initialize(consoleArray);
    //qDebug() << "before try block";

    try{
        newNode = parseFile(mainWindow->getOpenFile().toStdString());
        //qDebug() << "file has been parsed";
        //qDebug() << newNode;
        if(debug){
            std::string strArray[1];
            strArray[0]="-d";
            //qDebug() << "debugging";

            mainWindow->setMainValue(interpret(newNode, strArray, 1));
        }
        else{
            mainWindow->setMainValue(interpret(newNode, NULL, 0));
            //qDebug() << "runnjing";

        }
        //qDebug() << "out of try block";

        //mainWindow->setBuildSuccessful(true);
    }
    catch (RuntimeError e){
        mainWindow->printError(e);
        qDebug() << "error";
    }
    catch (SystemError e){
        mainWindow->printError(e);
        qDebug() << "error";
    }
    catch (Error e){
        mainWindow->printError(e);
         qDebug() << "error";
    }
    emit finished();
}
