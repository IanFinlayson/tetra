/* settingsmanager.cpp
 * code to manage saving, restoring, and querying settings */

#include <QtWidgets>
#include "settingsmanager.h"
#include "settingsdialog.h"

QSettings* SettingsManager::qset = NULL;

/* external referecne to the color schemes, 0 is default */
extern ColorScheme schemes[12];

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
    /* start with the generic monospace font */
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


QColor SettingsManager::foreground() {
    return qset->value("color/foreground", QColor(schemes[0].foreground)).value<QColor>();
}

QColor SettingsManager::background() {
    return qset->value("color/background", QColor(schemes[0].background)).value<QColor>();
}

QColor SettingsManager::keywords() {
    return qset->value("color/keywords", QColor(schemes[0].keywords)).value<QColor>();
}

QColor SettingsManager::types() {
    return qset->value("color/types", QColor(schemes[0].types)).value<QColor>();
}

QColor SettingsManager::values() {
    return qset->value("color/values", QColor(schemes[0].values)).value<QColor>();
}

QColor SettingsManager::comments() {
    return qset->value("color/comments", QColor(schemes[0].comments)).value<QColor>();
}

QColor SettingsManager::functions() {
    return qset->value("color/functions", QColor(schemes[0].functions)).value<QColor>();
}

QColor SettingsManager::termForeground() {
    return qset->value("color/termForeground", QColor(schemes[0].termForeground)).value<QColor>();
}

QColor SettingsManager::termBackground() {
    return qset->value("color/termBackground", QColor(schemes[0].termBackground)).value<QColor>();
}

QColor SettingsManager::linesForeground() {
    return qset->value("color/linesForeground", QColor(schemes[0].linesForeground)).value<QColor>();
}

QColor SettingsManager::linesBackground() {
    return qset->value("color/linesBackground", QColor(schemes[0].linesBackground)).value<QColor>();
}

void SettingsManager::setForeground(QColor color) {
    qset->setValue("color/foreground", color);
}

void SettingsManager::setBackground(QColor color) {
    qset->setValue("color/background", color); 
}

void SettingsManager::setKeywords(QColor color) {
    qset->setValue("color/keywords", color); 
}

void SettingsManager::setTypes(QColor color) {
    qset->setValue("color/types", color); 
}

void SettingsManager::setValues(QColor color) {
    qset->setValue("color/values", color); 
}

void SettingsManager::setComments(QColor color) {
    qset->setValue("color/comments", color); 
}

void SettingsManager::setFunctions(QColor color) {
    qset->setValue("color/functions", color); 
}

void SettingsManager::setTermForeground(QColor color) {
    qset->setValue("color/termForeground", color); 
}

void SettingsManager::setTermBackground(QColor color) {
    qset->setValue("color/termBackground", color);

}

void SettingsManager::setLinesBackground(QColor color) {
    qset->setValue("color/linesBackground", color); 
}

void SettingsManager::setLinesForeground(QColor color) {
    qset->setValue("color/linesForeground", color); 
}

