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
#include <QSettings>
#include <QSignalMapper>
#include <QThread>

#include "tetra.h"

#include "filerunner.h"
#include "replacedialog.h"
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

    Editor* currentEditor();

  public slots:
    /* when the file runner has output for us */
    void receiveOutput(QString text);

    /* when the file runner wants input from us */
    void getInput();

    /* when the console has input for us to pass to program */
    void receiveInput(QString text);

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
    void onTabChange(int);

    void on_actionRun_triggered();
    void exitRunMode();
    void on_actionDebug_triggered();
    void reportError(QString mesg, int line);

    void on_actionStep_triggered();
    void on_actionContinue_triggered();
    void on_actionNext_triggered();
    void on_actionStop_triggered();

    void hideSearch();
    void searchNext();
    void searchPrev();
    void doSearch(bool forward);
    void clearSearchColor(QString);
    void on_actionReplace_triggered();
    void saveMatchCase(int);

  private:
    void setupShortcuts();

    QSignalMapper* windowMapper;
    QString strippedName(const QString& fullFileName);

    bool maybeSave();
    bool buildSuccessful;

    QString mode;
    FileRunner* fileRunner;
    QThread* tetraThread;

    ReplaceDialog replaceDialog;
    QLabel* coords;

    void showSearch();

    void createStatusBar();

  protected:
    void closeEvent(QCloseEvent*);
};

#endif
