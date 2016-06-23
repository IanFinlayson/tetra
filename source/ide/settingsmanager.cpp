/* settingsmanager.cpp
 * code to manage saving, restoring, and querying settings */

#include <QtWidgets>
#include "settingsmanager.h"

QSettings* SettingsManager::qset = NULL;

void SettingsManager::init() {
    qset = new QSettings("Tetra", "Tetra 0.2");
}

int SettingsManager::tabWidth() {
    if (qset->contains("editor/tabs")) {
        return 8;
    } else {
        return 8;
    }
}

bool SettingsManager::lineNo() {
    return true;
}

bool SettingsManager::smartEdit() {
    return true;
}

QFont SettingsManager::font() {
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(12);
    return font;
}



