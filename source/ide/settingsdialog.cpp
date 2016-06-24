/* settingsdialog.cpp
 * code for the settings dialog */

#include <stdio.h>
#include <QFontDialog>

#include "settingsdialog.h"
#include "settingsmanager.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);

    /* set values to the default ones */
    tabWidth->setValue(SettingsManager::tabWidth());
    lineNo->setCheckState(SettingsManager::lineNo() ? Qt::Checked : Qt::Unchecked);
    smartEdit->setCheckState(SettingsManager::smartEdit() ? Qt::Checked : Qt::Unchecked);
    fontButton->setText(SettingsManager::font().family() + " | " +
            QString::number(SettingsManager::font().pointSize()));

    /* set up the font button */
    connect(fontButton, SIGNAL(released()),this, SLOT(askFont()));
}

void SettingsDialog::askFont() {
    /* ask the user what font they want */
    bool ok;
    QFont font = QFontDialog::getFont(&ok, SettingsManager::font(), this);

    /* if they clicked OK, set it */
    if (ok) {
        SettingsManager::setFont(font);
        fontButton->setText(SettingsManager::font().family() + " | " +
            QString::number(SettingsManager::font().pointSize()));
    } 
}


void SettingsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == buttonBox->button(QDialogButtonBox::Ok)) {
        SettingsManager::setTabWidth(tabWidth->value());
        SettingsManager::setLineNo(lineNo->checkState() == Qt::Checked);
        SettingsManager::setSmartEdit(smartEdit->checkState() == Qt::Checked);
        accept();

    } else if (button == buttonBox->button(QDialogButtonBox::Cancel)) {
        /* do not save any settings and reject */
        reject();
    }
}

