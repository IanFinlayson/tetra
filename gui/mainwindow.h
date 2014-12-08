#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QLayout>
#include <QMap>
#include <QThread>
#include <QSignalMapper>
#include "syntaxhighlighter.h"
#include "filerunner.h"
#include "threadwindow.h"
#include "../frontend/frontend.hpp"
#include "../interpreter/backend.hpp"

class Editor;
class FileRunner;

QT_BEGIN_NAMESPACE
class QPrinter;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool openProject();
    bool newProject();
    void showEditor();
    QString getOpenFile();
    void setMainValue(int);
    void setBuildSuccessful(bool);
    void printError(Error);
    void printMainValue();
    void runMode(bool);
    void debugMode(bool);
    void runFile();
    void quit();

private slots:
    void on_actionCopy_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    int on_actionPrint_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionPaste_triggered();
    void on_actionDelete_triggered();
    void on_actionSelect_All_triggered();
    void on_actionNew_triggered();
    void on_actionFind_triggered();
    void on_actionLine_Numbers_toggled(bool arg1);
    void on_actionMinimize_triggered();
    void on_actionLine_Numbers_triggered();
    void on_actionClear_Output_triggered();
    void on_actionDebug_toggled(bool arg1);
    void on_actionStop_triggered();

    void documentWasModified();
    void updateCoordinates();
    void exitRunMode();
    void on_actionRun_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QString openFile;

    void hideDisplay();
    void setupEditor();
    void setupShortcuts();

    QSignalMapper *windowMapper;
    void setupThreadMdi();
    
    
    Highlighter *highlighter;

    QString strippedName(const QString &fullFileName);

    bool maybeSave();
    int mainValue;
    bool buildSuccessful;

    QString mode;
    FileRunner *fileRunner;
    QThread *tetraThread;
private slots:
    //ThreadWindow *createThreadWindow();
    //setActiveSubWindow(QWidget *window);
    
    void on_actionExit_Debug_Mode_triggered();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
