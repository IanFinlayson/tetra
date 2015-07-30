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
#include "../interpreter/frontend.h"
#include "../interpreter/backend.h"

class Editor;
class FileRunner;
class DebugWindow;
QT_BEGIN_NAMESPACE
class QPrinter;
class Console;
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
    void on_actionDelete_triggered();
    void on_actionSelect_All_triggered();
    void on_actionStartProject_triggered( );
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
    void on_actionExit_Debug_Mode_triggered();

    DebugWindow *newThreadWindow();
    DebugWindow *activeThreadWindow();
    void setActiveSubWindow(QWidget *window);

    void on_actionStep_triggered();

    void on_actionContinue_triggered();

    void on_actionNext_triggered();

    void on_actionSet_Breakpoint_triggered();

    void on_actionRemove_Breakpoint_triggered();

private:
    Ui::MainWindow *ui;
    QString openFile;

    void showDisplay(bool arg1);

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

    void createStatusBar();
    int projectTabWidth;

    void hideUserInput(bool);
    void simulateStdIn(QString);
protected:
    void closeEvent(QCloseEvent *);

};

class Console: public VirtualConsole{
private:
   MainWindow *mainWindow;

public:
    Console(MainWindow *mainWindow) : VirtualConsole(){
        this->mainWindow = mainWindow;
    }
    std::string receiveStandardInput() const{
        return mainWindow->getUserInput();
    }
    void processStandardOutput(const std::string text) const{
        mainWindow->printOutput(QString::fromStdString(text));
    }
};


#endif // MAINWINDOW_H

