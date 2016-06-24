/* settingsmanager.cpp
 * code to manage saving, restoring, and querying settings */

#include <QtWidgets>
#include "settingsmanager.h"

QSettings* SettingsManager::qset = NULL;

void SettingsManager::init() {
    qset = new QSettings("tetra", "tetraide0.2");
}

int SettingsManager::tabWidth() {
    return qset->value("editor/tabWidth", 4).toInt() ;
}

bool SettingsManager::lineNo() {
    return qset->value("editor/lineNo", true).toBool() ;
}

bool SettingsManager::smartEdit() {
    return qset->value("editor/smartEdit", true).toBool() ;
}

QFont SettingsManager::font() {
    /* star with the generic monospace font */
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(12);

    return qset->value("editor/font", font).value<QFont>();
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
    qset->setValue("editor/font", font);
}

