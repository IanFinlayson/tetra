/* mainwindow.h
 * the main window for the application */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QSettings>
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
    void setMainValue(int);
    void setBuildSuccessful(bool);
    void printError(Error);
    void printMainValue();
    QString getOpenFile();
    void quit();

    QStatusBar getStatusBar();
    void setConsole(Console);
    void updateTitle();

    void doOpen(QString fname);

    Ui::MainWindow* ui;

  private slots:
    void on_actionCopy_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    int on_actionPrint_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionQuit_triggered();
    void on_actionRedo_triggered();
    void on_actionPaste_triggered();
    void on_actionNew_triggered();
    void on_actionFind_triggered();
    void on_actionClose_triggered();
    void on_actionAbout_Tetra_triggered();
    void on_actionDocumentation_triggered();
    void on_actionSettings_triggered();

    void documentWasModified();
    void updateCoordinates();

    void on_actionRun_triggered();
    void on_actionDebug_triggered();

    void on_actionStep_triggered();
    void on_actionContinue_triggered();
    void on_actionNext_triggered();
    void on_actionStop_triggered();

  private:

    void setupShortcuts();

    Editor* currentEditor();

    QSignalMapper* windowMapper;
    QString strippedName(const QString& fullFileName);

    bool maybeSave();
    int mainValue;
    bool buildSuccessful;

    QString mode;
    FileRunner* fileRunner;
    QThread* tetraThread;

    QLabel* coords;

    void createStatusBar();

  protected:
    void closeEvent(QCloseEvent*);
};

#endif  // MAINWINDOW_H
