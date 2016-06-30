/* filerunner.cpp
 * code to run or debug a program */

#include <QInputDialog> 
#include <QDebug>
#include <QMutex>

#include "console.h"
#include "filerunner.h"
#include "ideCommandObserver.h"

/* this function is defined in the Tetra interpreter code, linked as libtetra.a */
int interpret(Node* tree, int debug, int threads);

/* create the FileRunner and save the window it's associated with */
FileRunner::FileRunner(MainWindow* mainWindow) : VirtualConsole() {
    /* output */
    connect(this, SIGNAL(output(QString)), mainWindow, SLOT(receiveOutput(QString)));

    /* input */
    connect(this, SIGNAL(needInput()), mainWindow, SLOT(getInput()));

    /* save main window ref */
    this->mainWindow = mainWindow;
}

/* run or debugs file */
void FileRunner::runFile(bool debug) {
    ConsoleArray consoleArray;
    consoleArray.registerConsole(*this);

    Node* program_root;
    IDECommandObserver debugger = IDECommandObserver();
    TetraEnvironment::setObserver(debugger);
    TetraEnvironment::initialize(consoleArray);

    try {
        program_root = parseFile(mainWindow->getOpenFile().toStdString());

        if (debug) {
            interpret(program_root, true, 1);
        } else {
            interpret(program_root, false, 8);
        }

    } catch (RuntimeError e) {
        qDebug() << "RE: " << e.getMessage().c_str() << "\n";
    } catch (SystemError e) {
        qDebug() << "SE: " << e.getMessage().c_str() << "\n";
    } catch (Error e) {
        qDebug() << "E: " << e.getMessage().c_str() << "\n";
    }
    QThread::currentThread()->quit();
    emit finished();
}

/* this function is called from the interpreter when it needs string input from the user */
std::string FileRunner::receiveStandardInput() {
    /* tell the main window we need input */
    emit needInput();

    /* wait for the main thread to provide it */
    QMutex mutex;
    mutex.lock();
    inputReady.wait(&mutex);
    mutex.unlock();

    /* and now give it back to the interpreter */
    return myInput;
}

void FileRunner::processStandardOutput(const std::string& text) {
    /* send this string to the main window for display */
    QString qtext = QString(text.c_str());
    emit output(qtext);
}

/* this function is called from the main thread and unlocks the interpreter
 * thread which was waiting for input */
void FileRunner::receiveInput(QString input) {
    /* set member variable to waht main read */
    myInput = input.toStdString();

    /* wake up the thread waiting for input */
    inputReady.wakeAll();
}


