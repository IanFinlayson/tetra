#include "rundialog.h"
#include "ui_rundialog.h"

RunDialog::RunDialog(QWidget *parent):QDialog(parent), ui(new Ui::RunDialog){
    ui->setupUi(this);
    //setModal(true);
    setWindowTitle(tr("Run Program"));
    setFixedSize(275, 175);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->multipleThreadsCheckBox->setChecked(false);
    ui->numThreadsSpinBox->setEnabled(false);
}

RunDialog::~RunDialog()
{
    delete ui;
}

void RunDialog::on_multipleThreadsCheckBox_clicked(bool checked){
    ui->numThreadsSpinBox->setEnabled(checked);
}
