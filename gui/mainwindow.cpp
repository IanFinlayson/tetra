#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quitdialog.h"
#include "openappdialog.h"
#include "syntaxhighlighter.h"
#include "editor.h"
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    menuBar()->setNativeMenuBar(true);
    ui->setupUi(this);
    this->setWindowTitle("Tetra");

    connect(ui->input->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
    hideEditor();
    setupShortcuts();

    highlighter = new Highlighter(ui->input->document());


/*


    for(int i = 0; i < MaxRecentFiles; ++i){
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
    for(int i = 0; i < MaxRecentFiles; ++i){
        ui->menuFile->addAction(recentFileActs[i]);
    }
    updateRecentFileActions();*/
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::hideEditor(){
    ui->tetraFileLabel->hide();
    ui->outputLabel->hide();
    ui->input->hide();
    ui->output->hide();
    ui->lineEdit->hide();
    ui->enterButton->hide();
}

void MainWindow::showEditor(){
    ui->tetraFileLabel->show();
    ui->outputLabel->show();
    ui->input->show();
    ui->output->show();
    ui->lineEdit->show();
    ui->enterButton->show();

    QFont font = QFont("Monaco");
    font.setFixedPitch(true);
    font.setPointSize(12);
    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    ui->input->setTabStopWidth(tabStop * metrics.width(' '));
    ui->input->setFont(font);
    ui->output->setFont(font);
}

void MainWindow::setupShortcuts(){
    ui->actionNew->setShortcuts(QKeySequence::New);
    ui->actionCopy->setShortcuts(QKeySequence::Copy);
    ui->actionCut->setShortcuts(QKeySequence::Cut);
    ui->actionPaste->setShortcuts(QKeySequence::Paste);
    ui->actionDelete->setShortcuts(QKeySequence::Delete);
    ui->actionRedo->setShortcuts(QKeySequence::Redo);
    ui->actionUndo->setShortcuts(QKeySequence::Undo);
    ui->actionSave->setShortcuts(QKeySequence::Save);
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionSelect_All->setShortcuts(QKeySequence::SelectAll);
}

bool MainWindow::newProj(){
    bool projectCreated = false;
    on_actionNew_triggered();
    if(openFile != ""){
        projectCreated = true;
        showEditor();
    }
    return projectCreated;
}
bool MainWindow::openProj(){
    bool projectOpened = false;
    on_actionOpen_triggered();
    if(openFile != ""){
        projectOpened = true;
        showEditor();
    }
    return projectOpened;
}

void MainWindow::openRecentFile(){
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}


void MainWindow::loadFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->input->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
}

void MainWindow::setCurrentFile(const QString &fileName){
    openFile = fileName;
    setWindowFilePath(openFile);
    ui->tetraFileLabel->setText(strippedName(openFile));

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}
void MainWindow::updateRecentFileActions(){
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}
//-------------------------------------//



//-------------------------------------//
void MainWindow::documentWasModified(){
    setWindowModified(ui->input->document()->isModified());
}

bool MainWindow::maybeSave(){
    if (ui->input->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save){
            on_actionSave_triggered();
            return true;
        }
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
//-------------------------------------//

//-----------Menu Bar/Tool Bar Actions-----------//
void MainWindow::on_actionNew_triggered(){
    if (maybeSave()){
        QString filename = QFileDialog::getSaveFileName(this, tr("New Project"), "../../../..", "Tetra (*.ttr)");
        if(!filename.isEmpty()){
            ui->input->setPlainText("");
            openFile = filename;
            on_actionSave_triggered();
            ui->tetraFileLabel->setText(strippedName(openFile));
        }
    }
}

void MainWindow::on_actionSave_triggered(){
    QFile ttrFile(openFile);
    if(ttrFile.open(QFile::WriteOnly | QFile::Text)){
        QTextStream out(&ttrFile);

        out << ui->input->toPlainText();

        ttrFile.flush();
        ttrFile.close();
        //setCurrentFile(openFile);
    }
    else{
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Project As"), "../../../..", "Tetra (*.ttr)");
        if(!filename.isEmpty()){
            //setCurrentFile(openFile);
            on_actionSave_triggered();
        }
    }
}

void MainWindow::on_actionOpen_triggered(){
    if (maybeSave()){
        QString filename = QFileDialog::getOpenFileName(this, tr("Open Project"), "../../../..", "Tetra (*.ttr)");
        if(!filename.isEmpty()){
            QFile ttrFile(filename);
            if(ttrFile.open(QFile::ReadOnly | QFile::Text)){
                openFile = filename;
                QTextStream in(&ttrFile);
                QString fileText = in.readAll();
                ttrFile.close();

                ui->input->setPlainText(fileText);
                ui->tetraFileLabel->setText(strippedName(openFile));
            }
        }
    }
}
int MainWindow::on_actionPrint_triggered(){
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted){
        return -1;
    }

    QPainter painter;
    painter.begin(&printer);
    QFont f("Monaco");
    f.setStyleHint(QFont::Monospace);
    painter.setFont(f);
    painter.drawText(100, 100, 500, 500, Qt::AlignLeft|Qt::AlignTop, ui->input->toPlainText());

    painter.end();
    return 0;
}
void MainWindow::on_actionCut_triggered(){
    ui->input->cut();
}
void MainWindow::on_actionUndo_triggered(){
    ui->input->undo();
}
void MainWindow::on_actionRedo_triggered(){
    ui->input->redo();
}
void MainWindow::on_actionCopy_triggered(){
    ui->input->copy();
}
void MainWindow::on_actionPaste_triggered(){
    ui->input->paste();
}
void MainWindow::on_actionDelete_triggered(){
     QTextCursor textCursor = ui->input->textCursor();
     textCursor.insertText("");
}
void MainWindow::on_actionSelect_All_triggered(){
    ui->input->selectAll();
}
//-----------------------------------------------//
