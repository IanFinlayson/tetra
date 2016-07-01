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

    /* start timer */
    inputTimer = 0;
    programTimer.start();

    Node* program_root;
    IDECommandObserver debugger = IDECommandObserver();
    TetraEnvironment::setObserver(debugger);
    TetraEnvironment::initialize(consoleArray);

    try {
        program_root = parseFile(mainWindow->getOpenFile().toStdString().c_str());

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

    /* calculate elapsed running time */
    double seconds = (programTimer.elapsed() - inputTimer) / 1000.0;
    emit output("\nProgram Finished in " + QString::number(seconds, 'f', 2) + " seconds");

    emit finished();
}

/* this function is called from the interpreter when it needs string input from the user */
tstring FileRunner::receiveStandardInput() {
    /* tell the main window we need input */
    emit needInput();

    /* start the input timer so we count off time spent waiting for input */
    QElapsedTimer thisInputTime;
    thisInputTime.start();

    /* wait for the main thread to provide it */
    QMutex mutex;
    mutex.lock();
    inputReady.wait(&mutex);
    mutex.unlock();

    /* add this to input timer */
    inputTimer += thisInputTime.elapsed();

    /* and now give it back to the interpreter */
    return myInput.toStdString().c_str();
}

void FileRunner::processStandardOutput(const tstring& text) {
    /* send this string to the main window for display */
    QString qtext = QString(text.c_str());
    emit output(qtext);
}

/* this function is called from the main thread and unlocks the interpreter
 * thread which was waiting for input */
void FileRunner::receiveInput(QString input) {
    /* set member variable to waht main read */
    myInput = input.toStdString().c_str();

    /* wake up the thread waiting for input */
    inputReady.wakeAll();
}


