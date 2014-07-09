#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include "syntaxhighlighter.h"

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

private:
    Ui::MainWindow *ui;
    QString openFile;

    void hideEditor();
    void setupShortcuts();

    Highlighter *highlighter;

    QString strippedName(const QString &fullFileName);

    bool maybeSave();
};

#endif // MAINWINDOW_H
