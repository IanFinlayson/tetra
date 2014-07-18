#include "quitdialog.h"
#include "ui_quitdialog.h"
#include <QDesktopWidget>

QuitDialog::QuitDialog(QWidget *parent):QDialog(parent), ui(new Ui::QuitDialog){
    ui->setupUi(this);
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), qApp, SLOT(quit()));
    setModal(true);
    setWindowTitle(tr("Quit Tetra"));
    setFixedSize(240, 105);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

QuitDialog::~QuitDialog(){
    delete ui;
}

