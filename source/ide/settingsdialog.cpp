/* settingsdialog.cpp
 * code for the settings dialog */

#include <stdio.h>
#include <QColorDialog>
#include <QFontDialog>

#include "settingsdialog.h"
#include "settingsmanager.h"

/* they are a literal table given right here */
ColorScheme schemes[NUM_SCHEMES] = {
    /* name             fore      back      keys      types     vals
       comment   funcs     termfg    termbg    linefg    linebg    search
       error */
    {"Default", 0x000000, 0xf5f5f5, 0x004088, 0xb28c10, 0xaa0000, 0x1a5d11,
     0x73378c, 0xeeeeee, 0x424242, 0x404040, 0xe8e8e8, 0xffff7f, 0xee6666},
    {"Penumbra", 0x000000, 0xffffff, 0x7f0055, 0x7f0055, 0x0000c0, 0x3f7f5f,
     0x646464, 0x000000, 0xffffff, 0x646464, 0xffffff, 0xfff8a5, 0xee7096},
    {"Gedit", 0x4D525B, 0xffffff, 0xa52a2a, 0x2e8b57, 0xff00ff, 0x0000ff,
     0x4D525B, 0x4D525B, 0xffffff, 0x4D525B, 0xfbfbfb, 0xffff7f, 0xff9090},
    {"Lightcast", 0x3A4055, 0xF9F7F3, 0x5D96BF, 0xEAB25B, 0x519F50, 0x8783D1,
     0x5A647E, 0x2B2B2B, 0xE6E1DC, 0x444A5F, 0xF9F7F3, 0xaefbff, 0xffa9a9},
    {"Mocha", 0xd0c8c6, 0x3b3228, 0x8ab3b5, 0xf4bc87, 0xcb6077, 0xbeb55b,
     0xa89bb9, 0xd0c8c6, 0x3b3228, 0xd0c8c6, 0x3b3228, 0x878040, 0x9e4b5d},
    {"Monokai", 0xf8f8f8, 0x272727, 0xfa2772, 0x66d9ee, 0xae82ff, 0x76715e,
     0xa7e22e, 0xf8f8f8, 0x272727, 0xcfd0c2, 0x272727, 0x43909d, 0xaa1850},
    {"Oblivion", 0xd3d7cf, 0x2e3436, 0xad7fa8, 0x8ae234, 0xedd400, 0x858a85,
     0xd3d7cf, 0xd3d7cf, 0x2e3436, 0x858a85, 0x202020, 0xa18e00, 0x952c24},
    {"Old School", 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00,
     0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0xffff00, 0xff0000},
    {"Solarized Dark", 0x93a1a1, 0x002b36, 0x268bd2, 0xb58900, 0xdc322f,
     0x859900, 0x6c71c4, 0x93a1a1, 0x002b36, 0x657b83, 0x073642, 0x7d5c00,
     0x8a1e1c},
    {"Solarized Light", 0x586e75, 0xfdf6e3, 0x268bd2, 0xb58900, 0xdc322f,
     0x859900, 0x6c71c4, 0x586e75, 0xfdf6e3, 0x93a1a1, 0xeee8d5, 0xffe289,
     0xff9595},
    {"Studio", 0x000000, 0xffffff, 0x0502ff, 0x3191af, 0xbb0000, 0x008000,
     0x000000, 0xffffff, 0x000000, 0x3191af, 0xffffff, 0xffff7f, 0xff8b8b},
    {"Zenburn", 0xdcdccc, 0x3f3f3f, 0xf0dfaf, 0xc3bf9f, 0xdca3a3, 0x709080,
     0xdcdccc, 0xdcdccc, 0x3f3f3f, 0x709080, 0x3f3f3f, 0x9c916b, 0x9c7572}};

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setupUi(this);

    /* set values to the default ones */
    tabWidth->setValue(SettingsManager::tabWidth());
    lineNo->setCheckState(SettingsManager::lineNo() ? Qt::Checked
                                                    : Qt::Unchecked);
    smartEdit->setCheckState(SettingsManager::smartEdit() ? Qt::Checked
                                                          : Qt::Unchecked);
    fontButton->setText(SettingsManager::font().family() + " | " +
                        QString::number(SettingsManager::font().pointSize()));

    /* set up the font button */
    connect(fontButton, SIGNAL(released()), this, SLOT(askFont()));
    font_choice = SettingsManager::font();

    /* set up the color buttons so the color matches the current choice */
    foreground->setStyleSheet("background-color: " + SettingsManager::foreground().name());
    background->setStyleSheet("background-color: " + SettingsManager::background().name());
    keywords->setStyleSheet("background-color: " + SettingsManager::keywords().name());
    types->setStyleSheet("background-color: " + SettingsManager::types().name());
    values->setStyleSheet("background-color: " + SettingsManager::values().name());
    comments->setStyleSheet("background-color: " + SettingsManager::comments().name());
    functions->setStyleSheet("background-color: " + SettingsManager::functions().name());
    termForeground->setStyleSheet("background-color: " + SettingsManager::termForeground().name());
    termBackground->setStyleSheet("background-color: " + SettingsManager::termBackground().name());
    linesForeground->setStyleSheet("background-color: " + SettingsManager::linesForeground().name());
    linesBackground->setStyleSheet("background-color: " + SettingsManager::linesBackground().name());
    search->setStyleSheet("background-color: " + SettingsManager::search().name());
    error->setStyleSheet("background-color: " + SettingsManager::error().name());

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
    connect(search, SIGNAL(released()), this, SLOT(askColor()));
    connect(error, SIGNAL(released()), this, SLOT(askColor()));

    /* load the color schemes into the drop down */
    schemeChooser->addItem("Choose Scheme...");
    for (int i = 0; i < NUM_SCHEMES; i++) {
        schemeChooser->addItem(schemes[i].name);
    }
    connect(schemeChooser, SIGNAL(currentIndexChanged(int)),
            SLOT(selectTheme(int)));
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
    search->setPalette(QColor(cs->search));
    error->setPalette(QColor(cs->error));
}

void SettingsDialog::askColor() {
    QPushButton* it = (QPushButton*)sender();

    QColor selection = QColorDialog::getColor(
        it->palette().color(QWidget::backgroundRole()), this, "Select Color");

    if (selection.isValid()) {
        it->setPalette(selection);
    }
}

void SettingsDialog::askFont() {
    /* ask the user what font they want */
    bool ok;
    QFont font = QFontDialog::getFont(&ok, SettingsManager::font(), this);

    /* if they clicked OK, set it */
    if (ok) {
        fontButton->setText(font.family() + " | " +
                            QString::number(font.pointSize()));
        font_choice = font;
    }
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton* button) {
    if (button == buttonBox->button(QDialogButtonBox::Ok)) {
        SettingsManager::setTabWidth(tabWidth->value());
        SettingsManager::setLineNo(lineNo->checkState() == Qt::Checked);
        SettingsManager::setSmartEdit(smartEdit->checkState() == Qt::Checked);
        SettingsManager::setFont(font_choice);

        SettingsManager::setForeground(
            foreground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setBackground(
            background->palette().color(QWidget::backgroundRole()));
        SettingsManager::setKeywords(
            keywords->palette().color(QWidget::backgroundRole()));
        SettingsManager::setTypes(
            types->palette().color(QWidget::backgroundRole()));
        SettingsManager::setValues(
            values->palette().color(QWidget::backgroundRole()));
        SettingsManager::setComments(
            comments->palette().color(QWidget::backgroundRole()));
        SettingsManager::setFunctions(
            functions->palette().color(QWidget::backgroundRole()));
        SettingsManager::setTermForeground(
            termForeground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setTermBackground(
            termBackground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setLinesForeground(
            linesForeground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setLinesBackground(
            linesBackground->palette().color(QWidget::backgroundRole()));
        SettingsManager::setSearch(
            search->palette().color(QWidget::backgroundRole()));
        SettingsManager::setError(
            error->palette().color(QWidget::backgroundRole()));

        accept();

    } else if (button == buttonBox->button(QDialogButtonBox::Cancel)) {
        /* do not save any settings and reject */
        reject();
    }
}
