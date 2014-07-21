#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include "syntaxhighlighter.h"
#include "../frontend/frontend.hpp"

class Editor;

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
    bool openProj();
    bool newProj();
    void showEditor();
    QString getOpenFile();
    friend void* wrapperFunc(void*);


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
    void documentWasModified();
    void on_actionRun_triggered();
    void updateCoordinates();

    void on_actionFind_triggered();
    void on_actionLine_Numbers_toggled(bool arg1);

    void on_actionMinimize_triggered();

    void on_actionLine_Numbers_triggered();

    void on_actionClear_Output_triggered();

private:
    Ui::MainWindow *ui;
    QString openFile;

    void hideEditor();
    void setupShortcuts();

    Highlighter *highlighter;

    QString strippedName(const QString &fullFileName);

    bool maybeSave();
    int mainValue;
    bool buildSuccessful;
    Error buildError;
};

#endif // MAINWINDOW_H
