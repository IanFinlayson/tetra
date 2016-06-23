/* settingsmanager.cpp
 * code to manage saving, restoring, and querying settings */

#include <QtWidgets>
#include "settingsmanager.h"

QSettings* SettingsManager::qset = NULL;

void SettingsManager::init() {
    qset = new QSettings("tetra", "tetraide0.2");
}

int SettingsManager::tabWidth() {
    if (qset->contains("editor/tabWidth")) {
        return qset->value("editor/tabWidth").toInt() ;
    } else {
        return 4;
    }
}

bool SettingsManager::lineNo() {
    if (qset->contains("editor/lineNo")) {
        return qset->value("editor/lineNo").toBool() ;
    } else {
        return true;
    }
}

bool SettingsManager::smartEdit() {
    if (qset->contains("editor/smartEdit")) {
        return qset->value("editor/smartEdit").toBool() ;
    } else {
        return true;
    }
}

QFont SettingsManager::font() {
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(12);
    return font;
}


void SettingsManager::setTabWidth(int tabWidth) {
    qset->setValue("editor/tabWidth", tabWidth);
}

void SettingsManager::setLineNo(bool lineNo) {
    qset->setValue("editor/lineNo", lineNo);
}

void SettingsManager::setSmartEdit(bool smartEdit) {
    qset->setValue("editor/smartEdit", smartEdit);
}

void SettingsManager::setFont(QFont font) {

}

