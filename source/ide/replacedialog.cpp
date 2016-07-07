/* replacedialog.cpp
 * code for the replace dialog */

#include <QDebug>

#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget* parent) : QDialog(parent) {
    ui = new Ui::Replace;
    ui->setupUi(this);

    /* connect up the buttons */
    connect(ui->cancel, SIGNAL(released()), this, SLOT(cancel()));
    connect(ui->next, SIGNAL(released()), this, SLOT(next()));
    connect(ui->repl, SIGNAL(released()), this, SLOT(replace()));
    connect(ui->replaceAll, SIGNAL(released()), this, SLOT(replaceAll()));
}

/* close the dialog */
void ReplaceDialog::cancel() {
    close();
}

void ReplaceDialog::replace() {
    qDebug() << "repl";
}

void ReplaceDialog::replaceAll() {
    qDebug() << "all";
}

void ReplaceDialog::next() {
    qDebug() << "next";
}

