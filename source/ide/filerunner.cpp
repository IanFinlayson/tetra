/* filerunner.cpp
 * code to run or debug a program */

#include "filerunner.h"
#include <QDebug>
#include "ideCommandObserver.h"

/* this function is defined in the Tetra interpreter code, linked as libtetra.a */
int interpret(Node* tree, int debug, int threads);

/* create the FileRunner and save the window it's associated with */
FileRunner::FileRunner(MainWindow* mainWindow) {
    this->mainWindow = mainWindow;
}

/* run or debugs file */
void FileRunner::runFile(bool debug) {
    ConsoleArray consoleArray;
    Console console(mainWindow);
    consoleArray.registerConsole(console);
    Node* program_root;
    IDECommandObserver debugger = IDECommandObserver();
    TetraEnvironment::setObserver(debugger);
    TetraEnvironment::initialize(consoleArray);

    try {
        program_root = parseFile(mainWindow->getOpenFile().toStdString());
        if (debug) {
            mainWindow->setMainValue(interpret(program_root, 1, 1));
        } else {
            mainWindow->setMainValue(interpret(program_root, 0, 8));
        }
    } catch (RuntimeError e) {
        mainWindow->printError(e);
        qDebug() << "error";
    } catch (SystemError e) {
        mainWindow->printError(e);
        qDebug() << "error";
    } catch (Error e) {
        mainWindow->printError(e);
        qDebug() << "error";
    }
    emit finished();
}

