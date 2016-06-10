/* mainwindow.cpp
 * code for the main application window, actions, etc. */

#include "mainwindow.h"
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QScrollBar>
#include <QSignalMapper>
#include <QSize>
#include <QTabWidget>
#include <QThread>
#include <QtCore>
#include "editor.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    menuBar()->setNativeMenuBar(true);
    ui->setupUi(this);
    setWindowTitle(tr("Tetra"));
    statusBar()->showMessage("Ready.");
    setupShortcuts();

    tetraThread = new QThread;
    fileRunner = new FileRunner(this);
    mainValue = 0;

    connect(fileRunner, SIGNAL(finished()), this, SLOT(exitRunMode()));
    createStatusBar();

    coords = new QLabel("");
    statusBar()->addPermanentWidget(coords);
    
    ui->tabBar->setDocumentMode(true);
    ui->tabBar->setTabText(0, "Unsaved");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupShortcuts() {
    ui->actionNew->setShortcuts(QKeySequence::New);
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionSave->setShortcuts(QKeySequence::Save);
    ui->actionSave_As->setShortcuts(QKeySequence::SaveAs);
    ui->actionPrint->setShortcuts(QKeySequence::Print);
    ui->actionClose->setShortcuts(QKeySequence::Close);
    ui->actionQuit->setShortcuts(QKeySequence::Quit);

    ui->actionFind->setShortcuts(QKeySequence::Find);
    ui->actionReplace->setShortcuts(QKeySequence::Replace);
    ui->actionCut->setShortcuts(QKeySequence::Cut);
    ui->actionCopy->setShortcuts(QKeySequence::Copy);
    ui->actionPaste->setShortcuts(QKeySequence::Paste);
    ui->actionRedo->setShortcuts(QKeySequence::Redo);
    ui->actionUndo->setShortcuts(QKeySequence::Undo);

    ui->actionDocumentation->setShortcuts(QKeySequence::HelpContents);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    maybeSave();
    quit();
}

QString MainWindow::getOpenFile() {
    return currentEditor()->getOpenFile();
}


void MainWindow::quit() {
    QApplication::quit();
}

// gives stripped name of file (removes file path)
QString MainWindow::strippedName(const QString& fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

//-------------------------------------//
void MainWindow::documentWasModified() {
    // setWindowModified(ui->input->document()->isModified());
}

// asks user whether or not to save file before closing file
bool MainWindow::maybeSave() {
    return true;
}

void MainWindow::updateCoordinates() {
    coords->setText(currentEditor()->getCoordinates());
    currentEditor()->ensureCursorVisible();
}

void MainWindow::on_actionNew_triggered() {
    Editor* newEditor = new Editor;
    ui->tabBar->addTab(newEditor, "Unsaved");
    ui->tabBar->setCurrentWidget(newEditor);
}

void MainWindow::on_actionSave_triggered() {
    if (currentEditor()->save()) {
        QString full = currentEditor()->getOpenFile();
        QFileInfo info(full);
        ui->tabBar->setTabText(ui->tabBar->currentIndex(), info.fileName());
    }
}

void MainWindow::on_actionOpen_triggered() {
    QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Tetra (*.ttr)");
    Editor* newEditor = new Editor;
    if (newEditor->open(fname)) {
        QFileInfo info(fname);
        ui->tabBar->addTab(newEditor, info.fileName());
        ui->tabBar->setCurrentWidget(newEditor);
    } else {
        // TODO warn user of their failure
    }
}

int MainWindow::on_actionPrint_triggered() {
    QPrinter printer;

    QPrintDialog* dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted) {
        return -1;
    }

    QPainter painter;
    painter.begin(&printer);
    QFont f("Monaco");
    f.setStyleHint(QFont::Monospace);
    painter.setFont(f);
    painter.drawText(100, 100, 500, 500, Qt::AlignLeft | Qt::AlignTop,
                     currentEditor()->toPlainText());

    painter.end();
    return 0;
}

Editor* MainWindow::currentEditor() {
    return (Editor*)ui->tabBar->currentWidget();
}

void MainWindow::on_actionCut_triggered() {
    currentEditor()->cut();
}
void MainWindow::on_actionUndo_triggered() {
    currentEditor()->undo();
}
void MainWindow::on_actionQuit_triggered() {
    quit();
}
void MainWindow::on_actionRedo_triggered() {
    currentEditor()->redo();
}
void MainWindow::on_actionCopy_triggered() {
    currentEditor()->copy();
}
void MainWindow::on_actionPaste_triggered() {
    currentEditor()->paste();
}
void MainWindow::on_actionFind_triggered() {
}

//-----------------------------------------------//

// highlights line and prints error
void MainWindow::printError(Error e) {
    statusBar()->showMessage("Error.");
}

void MainWindow::on_actionStop_triggered() {
}

void MainWindow::setBuildSuccessful(bool buildSuccessful) {
}

void MainWindow::setMainValue(int mainValue) {
    this->mainValue = mainValue;
}

void MainWindow::exitRunMode() {
}

void MainWindow::debugMode(bool value) {
}

void MainWindow::on_actionRun_triggered(bool checked) {
}

// overrides output to output window
void MainWindow::printOutput(QString string) {
}

// overrides input to user input window
std::string MainWindow::getUserInput() {
    return "TODO";
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Ready.");
}

//--------------------Debugger Methods--------------------//
void MainWindow::on_actionStep_triggered() {
}

void MainWindow::on_actionContinue_triggered() {
}

void MainWindow::on_actionNext_triggered() {
}
