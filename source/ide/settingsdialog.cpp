/* settingsdialog.cpp
 * code for the settings dialog */

#include <stdio.h>
#include <QFontDialog>
#include <QColorDialog>

#include "settingsdialog.h"
#include "settingsmanager.h"

/* they are a literal table given right here */
ColorScheme schemes [NUM_SCHEMES] = {
    /* name             fore      back      keys      types     vals      comment   funcs     termfg    termbg    linefg    linebg */
    {"Default",         0x000000, 0xf5f5f5, 0x004088, 0xb28c10, 0xaa0000, 0x1a5d11, 0x73378c, 0xeeeeee, 0x424242, 0x404040, 0xe8e8e8},
    {"Penumbra",        0x000000, 0xffffff, 0x7f0055, 0x7f0055, 0x0000c0, 0x3f7f5f, 0x646464, 0x000000, 0xffffff, 0x646464, 0xffffff},
    {"Gedit",           0x5c616c, 0xffffff, 0xa52a2a, 0x2e8b57, 0xff00ff, 0x0000ff, 0x5c616c, 0x5c616c, 0xffffff, 0x5c616c, 0xfbfbfb},
    {"Lightcast",       0x3A4055, 0xF9F7F3, 0x5D96BF, 0xEAB25B, 0x519F50, 0x8783D1, 0x5A647E, 0x2B2B2B, 0xE6E1DC, 0x444A5F, 0xF9F7F3},
    {"Mocha",           0xd0c8c6, 0x3b3228, 0x8ab3b5, 0xf4bc87, 0xcb6077, 0xbeb55b, 0xa89bb9, 0xd0c8c6, 0x3b3228, 0xd0c8c6, 0x3b3228},
    {"Monokai",         0xf8f8f8, 0x272727, 0xfa2772, 0x66d9ee, 0xae82ff, 0x76715e, 0xa7e22e, 0xf8f8f8, 0x272727, 0xcfd0c2, 0x272727},
    {"Oblivion",        0xd3d7cf, 0x2e3436, 0xad7fa8, 0x8ae234, 0xedd400, 0x858a85, 0xd3d7cf, 0xd3d7cf, 0x2e3436, 0x858a85, 0x202020},
    {"Old School",      0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x000000},
    {"Solarized Dark",  0x93a1a1, 0x002b36, 0x268bd2, 0xb58900, 0xdc322f, 0x859900, 0x6c71c4, 0x93a1a1, 0x002b36, 0x657b83, 0x073642},
    {"Solarized Light", 0x586e75, 0xfdf6e3, 0x268bd2, 0xb58900, 0xdc322f, 0x859900, 0x6c71c4, 0x586e75, 0xfdf6e3, 0x93a1a1, 0xeee8d5},
    {"Studio",          0x000000, 0xffffff, 0x0502ff, 0x3191af, 0xbb0000, 0x008000, 0x000000, 0xffffff, 0x000000, 0x3191af, 0xffffff},
    {"Zenburn",         0xdcdccc, 0x3f3f3f, 0xf0dfaf, 0xc3bf9f, 0xdca3a3, 0x709080, 0xdcdccc, 0xdcdccc, 0x3f3f3f, 0x709080, 0x3f3f3f}
};


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

    /* set up the color buttons so the color matches the current choice */
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

    /* connect them to the color dialog action */
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

    /* load the color schemes into the drop down */
    schemeChooser->addItem("Choose Scheme...");
    for (int i = 0; i < NUM_SCHEMES; i++) {
        schemeChooser->addItem(schemes[i].name);
    }
    connect(schemeChooser, SIGNAL(currentIndexChanged(int)), SLOT(selectTheme(int)));
}

/* select a theme from the color scheme list */
void SettingsDialog::selectTheme(int index) {
    /* choice 0 is invalid, it is the "Choose Scheme" one */
    if (index == 0) {
        return;
    }

    /* look up the scheme from the array */
    ColorScheme* cs = &schemes[index - 1];

    /* apply it */
    foreground->setPalette(QColor(cs->foreground));
    background->setPalette(QColor(cs->background));
    keywords->setPalette(QColor(cs->keywords));
    types->setPalette(QColor(cs->types));
    values->setPalette(QColor(cs->values));
    comments->setPalette(QColor(cs->comments));
    functions->setPalette(QColor(cs->functions));
    termForeground->setPalette(QColor(cs->termForeground));
    termBackground->setPalette(QColor(cs->termBackground));
    linesForeground->setPalette(QColor(cs->linesForeground));
    linesBackground->setPalette(QColor(cs->linesBackground));
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

