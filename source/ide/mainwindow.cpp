#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openappdialog.h"
#include "editor.h"
#include <QDesktopWidget>
#include <QtCore>
#include <QFileDialog>
#include <QInputDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QScrollBar>
#include <QSignalMapper>
#include <QSize>
#include <QTabWidget>
#include <QThread>
#include "debugwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    menuBar()->setNativeMenuBar(true);
    ui->setupUi(this);
    setWindowTitle(tr("Tetra"));

    setupEditor();
    setupShortcuts();

    highlighter = new Highlighter(ui->input->document());

    QIcon icon(":graphics/resources/icons/tetra squares.ico");
    this->setWindowIcon(icon);

    tetraThread = new QThread;
    fileRunner = new FileRunner(this);
    mainValue = 0;

    connect(fileRunner, SIGNAL(finished()), this, SLOT(exitRunMode()));

    setupThreadMdi();
    createStatusBar();

    projectTabWidth=ui->input->getTabWidth();
}

MainWindow::~MainWindow(){
    delete ui;
}

//sets default values and connect signals/slots
void MainWindow::setupEditor(){
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

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionDelete->setEnabled(false);

    connect(ui->input, SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
    connect(ui->input, SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
    connect(ui->input, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
    connect(ui->input, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->input, SIGNAL(copyAvailable(bool)), ui->actionDelete, SLOT(setEnabled(bool)));
    connect(ui->input->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    connect(ui->input, SIGNAL(cursorPositionChanged()), this, SLOT(updateCoordinates()));

    ui->userInput->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);


    showDisplay(false);
}

void MainWindow::setupThreadMdi(){
    ui->threadMdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->threadMdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    windowMapper = new QSignalMapper;
    ui->threadMdi->hide();
}

void MainWindow::showDisplay(bool arg1){
    if(arg1){
        statusBar()->showMessage("Ready.");
    }
    ui->tetraFileLabel->setVisible(arg1);
    ui->outputLabel->setVisible(arg1);
    ui->input->setVisible(arg1);
    ui->output->setVisible(arg1);
    ui->cursorPosition->setVisible(arg1);
    ui->userInput->setVisible(arg1);
    ui->enterInputButton->setVisible(arg1);
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

void MainWindow::closeEvent(QCloseEvent *event){
    event->ignore();
    maybeSave();
    quit();
}


bool MainWindow::newProject(){
    bool projectCreated = false;
    on_actionTab_Width_triggered();
    on_actionNew_triggered();
    if(openFile != ""){
        projectCreated = true;
        showDisplay(true);
    }
    return projectCreated;
}

bool MainWindow::openProject(){
    bool projectOpened = false;
    on_actionOpen_triggered();
    if(openFile != ""){
        projectOpened = true;
        showDisplay(true);
    }
    return projectOpened;
}

void MainWindow::quit(){
    QMessageBox quitDialog;
    quitDialog.setWindowTitle("Quit Tetra");
    quitDialog.setText("Are you sure you want to quit Tetra?");
    quitDialog.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    quitDialog.setDefaultButton(QMessageBox::Cancel);
    if(quitDialog.exec() == QMessageBox::Ok){
        QApplication::quit();
    }
}

//gives stripped name of file (removes file path)
QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}
//-------------------------------------//



//-------------------------------------//
void MainWindow::documentWasModified(){
    setWindowModified(ui->input->document()->isModified());
}

//asks user whether or not to save file before closing file
bool MainWindow::maybeSave(){
    if (ui->input->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Discard | QMessageBox::Save);
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
    if(ui->input->checkLineHighlighted()){
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

//highlights line and prints error
void MainWindow::printError(Error e){
    buildSuccessful = false;
    ui->input->moveCursor(e.getLine());
    ui->output->insertPlainText(QString::number(e.getLine()) + ": " + QString::fromStdString(e.getMessage()) + "\n");
    ui->input->highlightLine(QColor(Qt::red));
    //statusBar()->showMessage("Error.");
}

void MainWindow::on_actionDebug_toggled(bool arg1){
    on_actionClear_Output_triggered();
    debugMode(arg1);
    ui->actionDebug->setEnabled(!arg1);
    ui->actionExit_Debug_Mode->setEnabled(arg1);
    if(arg1){
        statusBar()->showMessage ("Debugging.");
        maybeSave();
        fileRunner->moveToThread(tetraThread);
        tetraThread->start();
        qDebug() << "hey";
        QMetaObject::invokeMethod(fileRunner, "runFile", Qt::QueuedConnection, Q_ARG(bool,true));
        newThreadWindow();
    }
}


void MainWindow::on_actionStop_triggered(){
    exitRunMode();
    tetraThread->exit(0);
}

QString MainWindow::getOpenFile(){
    return this->openFile;
}
void MainWindow::setBuildSuccessful(bool buildSuccessful){
    this->buildSuccessful = buildSuccessful;
}
void MainWindow::setMainValue(int mainValue){
    this->mainValue = mainValue;
    //statusBar()->showMessage(QString::number(mainValue));
}
void MainWindow::exitRunMode(){
    ui->actionRun->setChecked(false);
    ui->input->setReadOnly(false);
    ui->actionRun->setEnabled(true);
    statusBar()->showMessage("Ready.");
}

void MainWindow::debugMode(bool value){
    ui->input->setVisible(!value);
    ui->threadMdi->setVisible(value); 
    ui->actionNew->setEnabled(!value);
    ui->actionPrint->setEnabled(!value);
    ui->actionOpen->setEnabled(!value);
    ui->actionLine_Numbers->setEnabled(!value);
    ui->menuEdit->setEnabled(!value);
    ui->menuBuild->setEnabled(!value);
    ui->actionBuild->setEnabled(!value);
    ui->actionRun->setEnabled(!value);
    ui->actionStop->setEnabled(!value);
    ui->actionContinue->setEnabled(value);
    ui->actionStep->setEnabled(value);
    ui->actionInterrupt->setEnabled(value);
    ui->actionNext->setEnabled(value);
    ui->actionSet_Breakpoint->setEnabled(value);
    ui->actionRemove_Breakpoint->setEnabled(value);

}

void MainWindow::on_actionRun_triggered(bool checked){
    on_actionClear_Output_triggered();
    ui->actionRun->setDisabled(true);
    ui->input->setReadOnly(true);
    if(checked){
        statusBar()->showMessage("Running.");
        maybeSave();
        fileRunner->moveToThread(tetraThread);
        tetraThread->start();
        QMetaObject::invokeMethod(fileRunner, "runFile", Qt::QueuedConnection, Q_ARG(bool,false));
    }
}

void MainWindow::on_actionExit_Debug_Mode_triggered()
{
    ui->actionDebug->setChecked(true);
    QMessageBox exitDebugBox;
    exitDebugBox.setWindowTitle("Stop Debugging");
    exitDebugBox.setText("Are you sure you want to stop debugging?");
    exitDebugBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    exitDebugBox.setDefaultButton(QMessageBox::Ok);
    if(exitDebugBox.exec() == QMessageBox::Ok){
       ui->actionDebug->setEnabled(true);
       ui->actionDebug->setChecked(false);
       debugMode(false);
       ui->threadMdi->closeAllSubWindows();
       statusBar()->showMessage("Ready.");
       this->tetraThread->disconnect();
    }
}

void MainWindow::on_actionTab_Width_triggered(){
    bool valueChanged;
    int tabWidth =  QInputDialog::getInt(this, "Tab Width", "Enter new tab width:", ui->input->getTabWidth(), 3, 10, 1, &valueChanged);
    if(valueChanged){
        ui->input->setTabWidth(tabWidth);
        this->projectTabWidth=tabWidth;
    }
}

//--------------------MDI Methods--------------------//
DebugWindow *MainWindow::newThreadWindow(){
    DebugWindow *newDebugWindow = new DebugWindow;
    newDebugWindow->setPlainText(ui->input->toPlainText());
    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWidget(newDebugWindow);
    subWindow->resize(QDesktopWidget().availableGeometry(this).size() * 0.2);
    subWindow->setWindowFlags(Qt::CustomizeWindowHint);
    ui->threadMdi->addSubWindow(subWindow);
    subWindow->show();
    return newDebugWindow;
}
DebugWindow *MainWindow::activeThreadWindow(){
    if (QMdiSubWindow *activeThreadWindow = ui->threadMdi->activeSubWindow()){
        return qobject_cast<DebugWindow *>(activeThreadWindow->widget());
    }
    return 0;
}
void MainWindow::setActiveSubWindow(QWidget *window){
    if (!window){
        return;
    }
    ui->threadMdi->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

//overrides output to output window
void MainWindow::printOutput(QString string){
    ui->output->insertPlainText(string);
    QScrollBar *outputSb = ui->output->verticalScrollBar();
    outputSb->setValue(outputSb->maximum());
}

//overrides input to user input window
std::string MainWindow::getUserInput(){
    QEventLoop loop; //loop waits for user to press enter button before retrieving text
    QObject::connect(ui->enterInputButton, SIGNAL(clicked()), &loop, SLOT(quit()));
    loop.exec();
    QString input = ui->userInput->toPlainText();
    ui->userInput->clear();
    return input.toStdString();
}

void MainWindow::createStatusBar(){
    statusBar()->showMessage("Welcome to Tetra.");
}


//--------------------Debugger Methods--------------------//
void MainWindow::on_actionStep_triggered(){
    hideUserInput(true);
    simulateStdIn("s");
    hideUserInput(false);
}

void MainWindow::on_actionContinue_triggered(){
    hideUserInput(true);
    simulateStdIn("c");
    hideUserInput(false);
}

void MainWindow::on_actionNext_triggered(){
    hideUserInput(true);
    simulateStdIn("n");
    hideUserInput(false);
}

//make texts in user input window invisible
void MainWindow::hideUserInput(bool hide){
    if(hide){ui->userInput->setStyleSheet("color: rgba(255, 255, 255, 0);");}
    else{ui->userInput->setStyleSheet("color: rgba(255, 255, 255, 255);");}
}

void MainWindow::on_actionSet_Breakpoint_triggered(){
    hideUserInput(true);
    simulateStdIn("b");

    bool valueChanged;
    int lineNumber =  QInputDialog::getInt(this, "Set Breakpoint", "Enter line number:", 1, 1, this->activeThreadWindow()->blockCount(), 1, &valueChanged);
    simulateStdIn(QString::number(lineNumber));
    hideUserInput(false);
}

//simulate std in by entering text to input box and pressing enter input button
void MainWindow::simulateStdIn(QString input){
    ui->userInput->setPlainText(input);
    ui->enterInputButton->animateClick();
}


void MainWindow::on_actionRemove_Breakpoint_triggered(){
    hideUserInput(true);
    simulateStdIn("r");

    bool valueChanged;
    int lineNumber =  QInputDialog::getInt(this, "Remove  Breakpoint", "Enter line number:", 1, 1, this->activeThreadWindow()->blockCount(), 1, &valueChanged);
    simulateStdIn(QString::number(lineNumber));
    hideUserInput(false);
}
