/* filerunner.cpp
 * code to run or debug a program */

#include <QDebug>
#include <QInputDialog>
#include <QMutex>

#include "console.h"
#include "filerunner.h"

/* this function is defined in the Tetra interpreter code, linked as libtetra.a
 */
int interpret(Node* tree, int debug, int threads);

/* create the FileRunner and save the window it's associated with */
FileRunner::FileRunner(MainWindow* mainWindow)
    : VirtualConsole() {
    /* output */
    connect(this, SIGNAL(output(QString)), mainWindow,
            SLOT(receiveOutput(QString)));

    /* input */
    connect(this, SIGNAL(needInput()), mainWindow, SLOT(getInput()));

    /* errors */
    connect(this, SIGNAL(errorSeen(QString, int)), mainWindow,
            SLOT(reportError(QString, int)));

    /* save main window ref */
    this->mainWindow = mainWindow;
}

/* run or debugs file */
void FileRunner::runFile(bool debug) {
    /* start timer */
    programTimer.start();

    /* assume normal completion */
    bool interrupted = false;
    bool error = false;

    Node* program_root;
    Environment::initialize();
    Environment::setConsole(this);

    try {
        program_root = parseFile(mainWindow->getOpenFile().toStdString().c_str());

        if (debug) {
            interpret(program_root, true, QThread::idealThreadCount());
        } else {
            interpret(program_root, false, QThread::idealThreadCount());
        }

    } catch (InterruptError e) {
        interrupted = true;
    } catch (Error e) {
        emit errorSeen(e.getMessage().toQ(), e.getLine());
        error = true;
    }
    QThread::currentThread()->quit();

    /* calculate elapsed running time */
    double seconds = programTimer.elapsed() / 1000.0;

    if (!interrupted && !error) {
        emit output("\nProgram finished in " + QString::number(seconds, 'f', 2) +
                    " seconds");
    } else if (!error) {
        emit output("\nProgram interupted after " +
                    QString::number(seconds, 'f', 2) + " seconds");
    }

    emit finished();
}

/* this function is called from the interpreter when it needs string input from
 * the user */
String FileRunner::receiveStandardInput() {
    /* tell the main window we need input */
    emit needInput();

    /* this will tell us if input was interrupted */
    inputInterrupted = false;

    /* wait for the main thread to provide it */
    QMutex mutex;
    mutex.lock();
    inputReady.wait(&mutex);
    mutex.unlock();

    /* if input was interrupted, don't return back */
    if (inputInterrupted) {
        throw InterruptError();
    }

    /* and now give it back to the interpreter */
    return myInput.toStdString().c_str();
}

void FileRunner::processStandardOutput(const String& text) {
    /* send this string to the main window for display */
    QString qtext = text.toQ();
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

/* stop the running program in its tracks */
void FileRunner::halt() {
    /* tell the program to stop when it can */
    Environment::halt();

    /* free up the input lock, which no longer matters */
    inputInterrupted = true;
    inputReady.wakeAll();
}
