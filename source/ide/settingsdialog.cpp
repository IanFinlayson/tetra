#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == buttonBox->button(QDialogButtonBox::Ok)) {
        /* TODO actually save the settings the user chose */
        accept();
    } else if (button == buttonBox->button(QDialogButtonBox::Cancel)) {
        /* do not save any settings and reject */
        reject();
    }
}

