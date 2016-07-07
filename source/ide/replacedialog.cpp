/* replacedialog.cpp
 * code for the replace dialog */

#include <QDebug>

#include "mainwindow.h"
#include "editor.h"
#include "settingsmanager.h"
#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget* parent) : QDialog(parent) {
    /* setup the GUI */
    ui = new Ui::Replace;
    ui->setupUi(this);

    /* save the main window */
    mainWindow = (MainWindow*) parent;

    /* connect up the buttons */
    connect(ui->cancel, SIGNAL(released()), this, SLOT(cancel()));
    connect(ui->next, SIGNAL(released()), this, SLOT(next()));
    connect(ui->repl, SIGNAL(released()), this, SLOT(replace()));
    connect(ui->replaceAll, SIGNAL(released()), this, SLOT(replaceAll()));

    /* make the replace all button default */
    ui->replaceAll->setDefault(true);

    /* set the initial checked ness from settings */
    ui->matchCase->setCheckState(SettingsManager::matchCase() ? Qt::Checked : Qt::Unchecked);
    connect(ui->matchCase, SIGNAL(stateChanged(int)), this, SLOT(saveMatchCase(int)));
}

void ReplaceDialog::updateSettings() {
    /* set the checked ness of this based on settings */
    ui->matchCase->setCheckState(SettingsManager::matchCase() ? Qt::Checked : Qt::Unchecked);
}

void ReplaceDialog::saveMatchCase(int) {
    /* save checked ness into settings */
    SettingsManager::setMatchCase(ui->matchCase->checkState() == Qt::Checked);
}

/* close the dialog */
void ReplaceDialog::cancel() {
    close();
}

/* TODO replace the next instance */
void ReplaceDialog::replace() {
    /* save checked ness into settings */
    SettingsManager::setMatchCase(ui->matchCase->checkState() == Qt::Checked);
    qDebug() << "repl";
}

/* replace all instances */
void ReplaceDialog::replaceAll() {
    /* save checked ness into settings */
    SettingsManager::setMatchCase(ui->matchCase->checkState() == Qt::Checked);
    qDebug() << "all";
}

/* called when it's time to do a search */
void ReplaceDialog::next() {
    if (ui->find->text().size() > 0) {
        mainWindow->currentEditor()->searchDir(ui->find->text(), true, ui->matchCase->checkState() == Qt::Checked);
    }
}

