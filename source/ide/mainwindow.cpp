/* mainwindow.cpp
 * code for the main application window, actions, etc. */

#include <QDesktopServices>
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

#include "mainwindow.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "editor.h"
#include "ui_mainwindow.h"
#include "ui_about.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    /* set the menu bar to work natively for systems with global bars */
    menuBar()->setNativeMenuBar(true);

    /* set up the ui elements for the window */
    ui->setupUi(this);

    /* set the title and icon */
    setWindowTitle(tr("Tetra [*]"));
    setWindowIcon(QIcon(":/icons/resources/icons/logo.svg"));

    /* set up the key shortcuts for the program */
    setupShortcuts(); 

    /* set up the thread and file runner for running programs */
    tetraThread = new QThread;
    fileRunner = new FileRunner(this);
    mainValue = 0;

    /* set up the status bar */
    statusBar()->showMessage("Ready.");
    coords = new QLabel("");
    statusBar()->addPermanentWidget(coords);
    updateCoordinates();

    /* set the tab bar in "document mode" and give the tab no name */
    ui->tabBar->setDocumentMode(true);
    ui->tabBar->setTabText(0, "Unsaved");

    /* set these as disabled (they are enabled dynamically */
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);

    /* set up connections of signals with the current editor */
    currentEditor()->setUpConnections(this);

    qRegisterMetaType<QTextBlock>("QTextBlock");
    qRegisterMetaType<QTextCursor>("QTextCursor");

    /* set up running so we know when we are done */
    connect(fileRunner, SIGNAL(finished()), this, SLOT(exitRunMode()));
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
    on_actionQuit_triggered();
}

QString MainWindow::getOpenFile() {
    return currentEditor()->getOpenFile();
}

// gives stripped name of file (removes file path)
QString MainWindow::strippedName(const QString& fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

/* load an initial file e.g. as an open file */
void MainWindow::doOpen(QString fname) {
    Editor* newEditor = new Editor;
    newEditor->setUpConnections(this);
    if (newEditor->open(fname)) {
        /* if there is one tab which is empty and not modified, close it first */
        if (ui->tabBar->count() == 1 && currentEditor()->isEmpty()) {
            currentEditor()->close();
            ui->tabBar->removeTab(ui->tabBar->currentIndex()); 
        }


        QFileInfo info(fname);
        ui->tabBar->addTab(newEditor, info.fileName());
        ui->tabBar->setCurrentWidget(newEditor);
        updateTitle();
    } else {
        QMessageBox warning;
        warning.setText("Could not open file '" + fname + "'");
        warning.setIconPixmap(QPixmap(":/icons/resources/icons/dialog-warning.svg"));
        warning.exec();
    }
}

void MainWindow::documentWasModified() {
    if (ui->tabBar->count() == 1) {
        setWindowModified(currentEditor()->document()->isModified());
    } else {
        updateTitle();
    }
}

void MainWindow::updateCoordinates() {
    coords->setText(currentEditor()->getCoordinates());
    currentEditor()->ensureCursorVisible();
}

void MainWindow::on_actionNew_triggered() {
    Editor* newEditor = new Editor;
    newEditor->setUpConnections(this);
    ui->tabBar->addTab(newEditor, "Unsaved");
    ui->tabBar->setCurrentWidget(newEditor);
    updateTitle();
}

void MainWindow::on_actionClose_triggered() {
    if (currentEditor()->document()->isModified()) {
        QMessageBox msgBox;
        msgBox.setText("The file has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setIconPixmap(QPixmap(":/icons/resources/icons/dialog-question.svg"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch (msgBox.exec()) {
            case QMessageBox::Save:
                /* try to save, if something goes wrong, bail */
                if (!currentEditor()->save()) {
                    return;
                }
                break;
            case QMessageBox::Discard:
                /* don't save! */
                break;
            case QMessageBox::Cancel:
                /* bail! */
                return;
        }
    }

    /* actually close the editor and tab */
    currentEditor()->close();
    ui->tabBar->removeTab(ui->tabBar->currentIndex());

    /* if that was the last tab, time to leave */
    if (ui->tabBar->count() == 0) {
        QApplication::quit();
    } else {
        updateTitle();
    }
}

void MainWindow::updateTitle() {
    QString full = currentEditor()->getOpenFile();
    QFileInfo info(full);

    if (full != "") {
        QString name = info.fileName();
        if (currentEditor()->document()->isModified()) {
            name += "*";
        }
        ui->tabBar->setTabText(ui->tabBar->currentIndex(), name);
    }

    if (ui->tabBar->count() == 1) {
        if (full != "") {
            setWindowTitle(info.fileName() + " [*]");
        } else {
            setWindowTitle("Tetra [*]");
        }
        setWindowModified(currentEditor()->document()->isModified());
    } else {
        setWindowTitle("Tetra [*]");
    } 
}

void MainWindow::on_actionSave_triggered() {
    if (currentEditor()->save()) {
        updateTitle();
    }
}

void MainWindow::on_actionSave_As_triggered() {
    if (currentEditor()->saveas()) {
        updateTitle();
    }
}

void MainWindow::on_actionOpen_triggered() {
    QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Tetra (*.ttr)");
    if (fname == "") {
        return;
    }

    doOpen(fname);
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
    QFont f = SettingsManager::font();
    painter.setFont(f);
    painter.drawText(100, 100, 500, 500, Qt::AlignLeft | Qt::AlignTop, currentEditor()->toPlainText());

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
    /* if there is one tab only, do a close */
    if (ui->tabBar->count() == 1) {
        on_actionClose_triggered();
    }

    /* check if there are unsaved tabs */
    int mod_count = 0;
    Editor* unsaved = NULL;
    for (int i = 0; i < ui->tabBar->count(); i++) {
        Editor* ed = (Editor*) ui->tabBar->widget(i);
        if (ed->document()->isModified()) {
            unsaved = ed;
            mod_count++;
        }
    }

    if (mod_count == 1) {
        QMessageBox msgBox;
        msgBox.setText("The file has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setIconPixmap(QPixmap(":/icons/resources/icons/dialog-question.svg"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch (msgBox.exec()) {
            case QMessageBox::Save:
                /* try to save, if something goes wrong, bail */
                if (!unsaved->save()) {
                    return;
                }
                break;
            case QMessageBox::Discard:
                /* don't save! */
                break;
            case QMessageBox::Cancel:
                /* bail! */
                return;
        }
    } else if (mod_count > 1) {
        QMessageBox msgBox;
        msgBox.setText("Files have been modified.");
        msgBox.setInformativeText("Do you want to save all changes?");
        msgBox.setIconPixmap(QPixmap(":/icons/resources/icons/dialog-question.svg"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch (msgBox.exec()) {
            case QMessageBox::Save:
                for (int i = 0; i < ui->tabBar->count(); i++) {
                    Editor* ed = (Editor*) ui->tabBar->widget(i);
                    if (!ed->save()) {
                        return;
                    }
                    break;
                }
            case QMessageBox::Discard:
                /* don't save! */
                break;
            case QMessageBox::Cancel:
                /* bail! */
                return;
        }
    }

    QApplication::quit();
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
    QMessageBox msgBox;
    msgBox.setText("TODO");
    msgBox.exec();
}

/* help and about functions */
void MainWindow::on_actionAbout_Tetra_triggered() {
    QDialog* about = new QDialog(0, 0);
    Ui_About about_ui;
    about_ui.setupUi(about);
    about->show();
}

void MainWindow::on_actionSettings_triggered() {
    SettingsDialog* prefs = new SettingsDialog(this);
    prefs->exec();

    /* update the settings of all open editors */
    for (int i = 0; i < ui->tabBar->count(); i++) {
        Editor* ed = (Editor*) ui->tabBar->widget(i);
        ed->updateSettings();
    }

    /* update the settings for the console */
    ui->console->updateSettings();
}


void MainWindow::on_actionDocumentation_triggered() {
    QDesktopServices::openUrl(QUrl("http://tetra-lang.org/ide-reference"));
}

/* run and debug functions */
void MainWindow::on_actionDebug_triggered() {
    QMessageBox msgBox;
    msgBox.setText("TODO");
    msgBox.exec();
}

void MainWindow::receiveOutput(QString text) {
    ui->console->write(text);
}

void MainWindow::on_actionRun_triggered() {
    /* if it's not saved, we can't run */
    if (currentEditor()->save()) {
        updateTitle();
    } else {
        return;
    }

    /* make sure the bottom dock is shown */
    ui->dock->show();
    ui->console->document()->setPlainText("");

    /* disable run again */
    ui->actionRun->setDisabled(true);
    statusBar()->showMessage("Running.");

    /* start the worker thread which runs the programs */
    fileRunner->moveToThread(tetraThread);
    tetraThread->start();
    QMetaObject::invokeMethod(fileRunner, "runFile", Qt::QueuedConnection, Q_ARG(bool,false));
}

/* we were requested to get input from the running program */
void MainWindow::getInput() {
    bool ok;
    QString text;
    do {
        text = QInputDialog::getText(this, tr("Enter Input"), tr("Enter Input"), QLineEdit::Normal, "", &ok);
    } while (!ok);

    qDebug() << "main sees input as " << text << "\n";
    fileRunner->receiveInput(text);
}

/* finish running this */
void MainWindow::exitRunMode(){
    tetraThread->wait();
    ui->actionRun->setEnabled(true);
    statusBar()->showMessage("Ready.");
}

/* debugger functions */
void MainWindow::on_actionStep_triggered() {
    QMessageBox msgBox;
    msgBox.setText("TODO");
    msgBox.exec();
}

void MainWindow::on_actionContinue_triggered() {
    QMessageBox msgBox;
    msgBox.setText("TODO");
    msgBox.exec();
}

void MainWindow::on_actionNext_triggered() {
    QMessageBox msgBox;
    msgBox.setText("TODO");
    msgBox.exec();
}

void MainWindow::on_actionStop_triggered() {
    QMessageBox msgBox;
    msgBox.setText("TODO");
    msgBox.exec();
}

