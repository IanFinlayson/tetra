#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quitdialog.h"
#include "openappdialog.h"
#include "editor.h"
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QScrollBar>
#include <QSize>
#include <pthread.h>

int interpret(const Node* tree);

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow), buildError("No Error", 0){
    menuBar()->setNativeMenuBar(true);
    ui->setupUi(this);
    setWindowTitle(tr("Tetra"));

    connect(ui->input->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    connect(ui->input, SIGNAL(cursorPositionChanged()), this, SLOT(updateCoordinates()));
    
    hideEditor();
    setupShortcuts();

    highlighter = new Highlighter(ui->input->document());
    QIcon icon(":graphics/Tetra Resources/icons/tetra squares.ico");
    this->setWindowIcon(icon);

    //mainValue = 0;
    //buildSuccessful = true;

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::hideEditor(){
    ui->tetraFileLabel->hide();
    ui->outputLabel->hide();
    ui->input->hide();
    ui->output->hide();
    ui->cursorPosition->hide();
}

void MainWindow::showEditor(){
    ui->tetraFileLabel->show();
    ui->outputLabel->show();
    ui->input->show();
    ui->output->show();
    ui->cursorPosition->show();

    QFont font = QFont("Monaco");
    font.setFixedPitch(true);
    font.setPointSize(12);
    font.setStyleHint(QFont::TypeWriter);

    QFontMetrics metrics(font);
    ui->input->setFont(font);
    ui->output->setFont(font);

    ui->input->ensureCursorVisible();
    ui->input->setCenterOnScroll(true);

    ui->cursorPosition->setAlignment(Qt::AlignRight);
    //ui->gridLayout->
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


//wrapper function for parseFile and interpret functions
void* wrapperFunc(void* arg1){
    MainWindow* tetraProg = static_cast<MainWindow*>(arg1);
    Node* newNode;
    try{
        newNode = parseFile(tetraProg->openFile.toStdString());
        tetraProg->mainValue = interpret(newNode);
        tetraProg->buildSuccessful = true;
    }
    catch (Error e){
        tetraProg->buildError = e;
        tetraProg->buildSuccessful = false;
    }
    return NULL;
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

void MainWindow::updateCoordinates(){
    ui->cursorPosition->setText(ui->input->getCoordinates());
    ui->input->ensureCursorVisible();
    if(ui->input->checkLineHighlighted){
        ui->input->unhighlightLine();
    }
}

//-----------Menu Bar/Tool Bar Actions-----------//
void MainWindow::on_actionNew_triggered(){
    if (maybeSave()){
        QString filename = QFileDialog::getSaveFileName(this, tr("New Project"), "../../../..", "Tetra (*.ttr)");
        if(!filename.isEmpty()){
            ui->input->setPlainText("");
            openFile = filename;
            on_actionSave_triggered();
            ui->tetraFileLabel->setText(strippedName(openFile));
            ui->output->setPlainText("");
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
    }
    else{
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Project As"), "../../../..", "Tetra (*.ttr)");
        if(!filename.isEmpty()){
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
                ui->output->clear();
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
     textCursor.removeSelectedText();
}
void MainWindow::on_actionSelect_All_triggered(){
    ui->input->selectAll();
}
void MainWindow::on_actionFind_triggered(){
  //  ui->input->find();
}
void MainWindow::on_actionRun_triggered(){
    pthread_t ttrThread;
    pthread_create(&ttrThread, NULL, (void*(*)(void*))wrapperFunc, this);\
    pthread_join(ttrThread, NULL);

    if(buildSuccessful){
        ui->output->insertPlainText(QString::number(mainValue)+ "\n");
    }
    else{
        ui->output->insertPlainText(QString::number(buildError.getLine()) + ": " + QString::fromStdString(buildError.getMessage()) + "\n");
        ui->input->highlightLine(QColor(Qt::red), buildError.getLine());
    }
}
void MainWindow::on_actionLine_Numbers_toggled(bool arg1){
    if(arg1 == true){
        ui->input->showLineNumbers(true);
    }
    else{
        ui->input->showLineNumbers(false);
    }
}
void MainWindow::on_actionMinimize_triggered(){
    this->showMinimized();
}
void MainWindow::on_actionLine_Numbers_triggered(){
    this->showFullScreen();
}
void MainWindow::on_actionClear_Output_triggered(){
    ui->output->clear();
}

//-----------------------------------------------//















