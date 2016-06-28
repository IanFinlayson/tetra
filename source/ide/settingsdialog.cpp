/* settingsdialog.cpp
 * code for the settings dialog */

#include <stdio.h>
#include <QFontDialog>
#include <QColorDialog>

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
    font_choice = SettingsManager::font();

    /* set up the color buttons */
    foreground->setPalette(SettingsManager::foreground());
    background->setPalette(SettingsManager::background());
    keywords->setPalette(SettingsManager::keywords());
    types->setPalette(SettingsManager::types());
    values->setPalette(SettingsManager::values());
    comments->setPalette(SettingsManager::comments());
    functions->setPalette(SettingsManager::functions());
    termForeground->setPalette(SettingsManager::termForeground());
    termBackground->setPalette(SettingsManager::termBackground());
    linesForeground->setPalette(SettingsManager::linesForeground());
    linesBackground->setPalette(SettingsManager::linesBackground());

    connect(foreground, SIGNAL(released()), this, SLOT(askColor()));
    connect(background, SIGNAL(released()), this, SLOT(askColor()));
    connect(keywords, SIGNAL(released()), this, SLOT(askColor()));
    connect(types, SIGNAL(released()), this, SLOT(askColor()));
    connect(values, SIGNAL(released()), this, SLOT(askColor()));
    connect(comments, SIGNAL(released()), this, SLOT(askColor()));
    connect(functions, SIGNAL(released()), this, SLOT(askColor()));
    connect(termForeground, SIGNAL(released()), this, SLOT(askColor()));
    connect(termBackground, SIGNAL(released()), this, SLOT(askColor()));
    connect(linesForeground, SIGNAL(released()), this, SLOT(askColor()));
    connect(linesBackground, SIGNAL(released()), this, SLOT(askColor()));
}

void SettingsDialog::askColor() {
    QPushButton* it = (QPushButton*) sender();

    QColor selection = QColorDialog::getColor(it->palette().color(QWidget::backgroundRole()), this, "Select Color");
    it->setPalette(selection);
}

void SettingsDialog::askFont() {
    /* ask the user what font they want */
    bool ok;
    QFont font = QFontDialog::getFont(&ok, SettingsManager::font(), this);

    /* if they clicked OK, set it */
    if (ok) {
        fontButton->setText(font.family() + " | " + QString::number(font.pointSize()));
        font_choice = font;
    } 
}


void SettingsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == buttonBox->button(QDialogButtonBox::Ok)) {
        SettingsManager::setTabWidth(tabWidth->value());
        SettingsManager::setLineNo(lineNo->checkState() == Qt::Checked);
        SettingsManager::setSmartEdit(smartEdit->checkState() == Qt::Checked);
        SettingsManager::setFont(font_choice);

        SettingsManager::setForeground(foreground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setBackground(background->palette().color(QWidget::backgroundRole()));
        SettingsManager::setKeywords(keywords->palette().color(QWidget::backgroundRole()));
        SettingsManager::setTypes(types->palette().color(QWidget::backgroundRole()));
        SettingsManager::setValues(values->palette().color(QWidget::backgroundRole()));
        SettingsManager::setComments(comments->palette().color(QWidget::backgroundRole()));
        SettingsManager::setFunctions(functions->palette().color(QWidget::backgroundRole()));
        SettingsManager::setTermForeground(termForeground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setTermBackground(termBackground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setLinesForeground(linesForeground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setLinesBackground(linesBackground->palette().color(QWidget::backgroundRole()));

        accept();

    } else if (button == buttonBox->button(QDialogButtonBox::Cancel)) {
        /* do not save any settings and reject */
        reject();
    }
}

