/* mainwindow.h
 * the main window for the application */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMap>
#include <QPlainTextEdit>
#include <QSignalMapper>
#include <QThread>

#include "../interpreter/backend.h"
#include "../interpreter/frontend.h"
#include "filerunner.h"
#include "syntaxhighlighter.h"

class Editor;
class FileRunner;

QT_BEGIN_NAMESPACE
class QPrinter;
class Console;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();
  bool openProject();
  bool newProject();
  bool startProject();
  QString getOpenFile();
  void setMainValue(int);
  void setBuildSuccessful(bool);
  void printError(Error);
  void printMainValue();
  void runMode(bool);
  void debugMode(bool);
  void runFile();
  void quit();

  void printOutput(QString);
  std::string getUserInput();
  QStatusBar getStatusBar();
  void setConsole(Console);
 private slots:
  void on_actionCopy_triggered();
  void on_actionSave_triggered();
  void on_actionOpen_triggered();
  int on_actionPrint_triggered();
  void on_actionCut_triggered();
  void on_actionUndo_triggered();
  void on_actionQuit_triggered();
  void on_actionRedo_triggered();
  void on_actionPaste_triggered();
  void on_actionNew_triggered();
  void on_actionFind_triggered();
  void on_actionStop_triggered();

  void documentWasModified();
  void updateCoordinates();

  void exitRunMode();
  void on_actionRun_triggered(bool checked);
  void on_actionStep_triggered();
  void on_actionContinue_triggered();
  void on_actionNext_triggered();

 private:
  Ui::MainWindow* ui;
  QString openFile;

  void showDisplay(bool arg1);

  void setupEditor();
  void setupShortcuts();

  QSignalMapper* windowMapper;
  void setupThreadMdi();

  Highlighter* highlighter;

  QString strippedName(const QString& fullFileName);

  bool maybeSave();
  int mainValue;
  bool buildSuccessful;

  QString mode;
  FileRunner* fileRunner;
  QThread* tetraThread;

  QLabel* coords;

  void createStatusBar();
  int projectTabWidth;

 protected:
  void closeEvent(QCloseEvent*);
};

class Console : public VirtualConsole {
 private:
  MainWindow* mainWindow;

 public:
  Console(MainWindow* mainWindow) : VirtualConsole() {
    this->mainWindow = mainWindow;
  }
  std::string receiveStandardInput() const {
    return mainWindow->getUserInput();
  }
  void processStandardOutput(const std::string text) const {
    mainWindow->printOutput(QString::fromStdString(text));
  }
};

#endif  // MAINWINDOW_H
