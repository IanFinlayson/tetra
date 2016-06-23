/* settingsdialog.cpp
 * code for the settings dialog */

#include "settingsdialog.h"
#include "settingsmanager.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);

    /* set values to the default ones */
    tabWidth->setValue(SettingsManager::tabWidth());
    lineNo->setCheckState(SettingsManager::lineNo() ? Qt::Checked : Qt::Unchecked);
    smartEdit->setCheckState(SettingsManager::smartEdit() ? Qt::Checked : Qt::Unchecked);
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

