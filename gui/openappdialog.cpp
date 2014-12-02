#include "openappdialog.h"
#include "ui_openappdialog.h"
#include "mainwindow.h"
#include "quitdialog.h"
#include <QFileDialog>
#include <QPushButton>
#include <QtCore>
#include <QDesktopWidget>

OpenAppDialog::OpenAppDialog(QWidget *parent):QDialog(parent), ui(new Ui::OpenAppDialog){
    this->mainwindow = NULL;
    ui->setupUi(this);
    QPixmap tetraLogo(":/graphics/Tetra Resources/logo.png");
    ui->logo->setPixmap(tetraLogo);
    ui->logo->setAlignment(Qt::AlignCenter);

    setModal(true);
    setWindowTitle(tr("Welcome to Tetra"));
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

OpenAppDialog::~OpenAppDialog(){
    delete ui;
}

void OpenAppDialog::setMainWindow(MainWindow *mainwindow){
    this->mainwindow = mainwindow;
}

void OpenAppDialog::on_quitAppButton_clicked(){
    QuitDialog quitdialog;
    quitdialog.exec();

}
void OpenAppDialog::on_newProjButton_clicked(){
    bool projectCreated = mainwindow->newProject();
    if(projectCreated){
        this->close();
    }
}
void OpenAppDialog::on_openProjButton_clicked(){
    bool projectOpened = mainwindow->openProject();
    if(projectOpened){
        this->close();
    }
}

void OpenAppDialog::keyPressEvent(QKeyEvent *e){
    e->ignore();
    return;
}
