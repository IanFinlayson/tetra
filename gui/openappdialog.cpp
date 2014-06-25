#include "openappdialog.h"
#include "ui_openappdialog.h"
#include "mainwindow.h"
#include "quitdialog.h"
#include <QFileDialog>
#include <QPushButton>
#include <QtCore>
#include <QDesktopWidget>

OpenAppDialog::OpenAppDialog(QWidget *parent):QDialog(parent), ui(new Ui::OpenAppDialog){
    ui->setupUi(this);
    QPixmap tetraLogo(":/graphics/Tetra Resources/logo.png");
    ui->logo->setPixmap(tetraLogo);

    this->setModal(true);
    this->setWindowTitle("Welcome to Tetra");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    this->setFixedSize(880, 350);
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
    bool projectCreated = mainwindow->newProj();
    if(projectCreated){
        this->close();
    }
}
void OpenAppDialog::on_openProjButton_clicked(){
    bool projectOpened = mainwindow->openProj();
    if(projectOpened){
        this->close();
    }
}
