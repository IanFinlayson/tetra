/* filerunner.cpp
 * code to run or debug a program */

#include <QDebug>

#include "console.h"
#include "filerunner.h"
#include "ideCommandObserver.h"

/* this function is defined in the Tetra interpreter code, linked as libtetra.a */
int interpret(Node* tree, int debug, int threads);

/* create the FileRunner and save the window it's associated with */
FileRunner::FileRunner(MainWindow* mainWindow) : VirtualConsole() {
    this->mainWindow = mainWindow;
    connect(this, SIGNAL(output(QString)), mainWindow, SLOT(receiveOutput(QString)));
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

/* TODO change this to use the actual console itself the difficulty in that is
 * that we can't return right away like we must for the interface... */
std::string FileRunner::receiveStandardInput() {

    return "42";

    /*
    while (true) {
        bool ok;
        QString text = QInputDialog::getText(parent, "Enter Input", "Enter Input", QLineEdit::Normal, "", &ok);
        if (ok) {
            return text.toStdString();
        }
    } */
}

void FileRunner::processStandardOutput(const std::string& text) {
    QString qtext = QString(text.c_str());
    emit output(qtext);
}

