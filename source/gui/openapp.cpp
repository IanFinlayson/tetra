/*#include "openappdialog.h"
#include "ui_openappdialog.h"
#include "mainwindow.h"
#include "quitdialog.h"

OpenAppDialog::OpenAppDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenAppDialog)
{
    ui->setupUi(this);

    QPixmap tetraLogo("/Users/Shehan/UMW/Programming/SSI/Tetra/Tetra Resources/logo.png");
    ui->logo->setPixmap(tetraLogo);
}

OpenAppDialog::~OpenAppDialog()
{
    delete ui;
}

void OpenAppDialog::on_quitAppButton_clicked(){
    QuitDialog quitdialog;
    quitdialog.setModal(true);
    quitdialog.setWindowTitle("Quit Tetra");
    quitdialog.setFixedSize(240, 105);
    quitdialog.exec();
}
*/
