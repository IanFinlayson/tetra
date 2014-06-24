#include "quitdialog.h"
#include "ui_quitdialog.h"
#include <QDesktopWidget>

QuitDialog::QuitDialog(QWidget *parent):QDialog(parent), ui(new Ui::QuitDialog){
    ui->setupUi(this);
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), qApp, SLOT(quit()));
    this->setModal(true);
    this->setWindowTitle("Quit Tetra");
    this->setFixedSize(240, 105);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

QuitDialog::~QuitDialog(){
    delete ui;
}

