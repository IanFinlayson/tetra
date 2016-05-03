#include "filerunner.h"
#include <QDebug>
#include "ideCommandObserver.h"

int interpret(Node* tree, int debug, int threads);

FileRunner::FileRunner(MainWindow* mainWindow) {
  this->mainWindow = mainWindow;
}

// run or debugs file
void FileRunner::runFile(bool debug) {
  ConsoleArray consoleArray;
  Console console(mainWindow);
  consoleArray.registerConsole(console);
  Node* newNode;
  IDECommandObserver debugger = IDECommandObserver();
  TetraEnvironment::setObserver(debugger);
  TetraEnvironment::initialize(consoleArray);

  try {
    newNode = parseFile(mainWindow->getOpenFile().toStdString());
    if (debug) {
      mainWindow->setMainValue(interpret(newNode, 1, 1));
    } else {
      mainWindow->setMainValue(interpret(newNode, 0, 8));
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
