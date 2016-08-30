/* settingsmanager.cpp
 * code to manage saving, restoring, and querying settings */

#include "settingsmanager.h"
#include <QtWidgets>
#include "settingsdialog.h"

QSettings* SettingsManager::settingsManager = NULL;

/* external referecne to the color schemes, 0 is default */
extern ColorScheme schemes[12];

void SettingsManager::init() {
    settingsManager = new QSettings("tetra", "tetraide0.2");
}

int SettingsManager::tabWidth() {
    return settingsManager->value("editor/tabWidth", 4).toInt();
}

bool SettingsManager::lineNo() {
    return settingsManager->value("editor/lineNo", true).toBool();
}

bool SettingsManager::smartEdit() {
    return settingsManager->value("editor/smartEdit", true).toBool();
}

bool SettingsManager::matchCase() {
    return settingsManager->value("editor/matchCase", true).toBool();
}

QFont SettingsManager::font() {
    /* start with the generic monospace font */
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(12);

    return settingsManager->value("editor/font", font).value<QFont>();
}

void SettingsManager::setTabWidth(int tabWidth) {
    settingsManager->setValue("editor/tabWidth", tabWidth);
}

void SettingsManager::setLineNo(bool lineNo) {
    settingsManager->setValue("editor/lineNo", lineNo);
}

void SettingsManager::setSmartEdit(bool smartEdit) {
    settingsManager->setValue("editor/smartEdit", smartEdit);
}

void SettingsManager::setFont(QFont font) {
    settingsManager->setValue("editor/font", font);
}

void SettingsManager::setMatchCase(bool mc) {
    settingsManager->setValue("editor/matchCase", mc);
}

QColor SettingsManager::foreground() {
    return settingsManager->value("color/foreground", QColor(schemes[0].foreground))
        .value<QColor>();
}

QColor SettingsManager::background() {
    return settingsManager->value("color/background", QColor(schemes[0].background))
        .value<QColor>();
}

QColor SettingsManager::keywords() {
    return settingsManager->value("color/keywords", QColor(schemes[0].keywords))
        .value<QColor>();
}

QColor SettingsManager::types() {
    return settingsManager->value("color/types", QColor(schemes[0].types)).value<QColor>();
}

QColor SettingsManager::values() {
    return settingsManager->value("color/values", QColor(schemes[0].values)).value<QColor>();
}

QColor SettingsManager::comments() {
    return settingsManager->value("color/comments", QColor(schemes[0].comments))
        .value<QColor>();
}

QColor SettingsManager::functions() {
    return settingsManager->value("color/functions", QColor(schemes[0].functions))
        .value<QColor>();
}

QColor SettingsManager::termForeground() {
    return settingsManager->value("color/termForeground", QColor(schemes[0].termForeground))
        .value<QColor>();
}

QColor SettingsManager::termBackground() {
    return settingsManager->value("color/termBackground", QColor(schemes[0].termBackground))
        .value<QColor>();
}

QColor SettingsManager::linesForeground() {
    return settingsManager
        ->value("color/linesForeground", QColor(schemes[0].linesForeground))
        .value<QColor>();
}

QColor SettingsManager::linesBackground() {
    return settingsManager
        ->value("color/linesBackground", QColor(schemes[0].linesBackground))
        .value<QColor>();
}

QColor SettingsManager::search() {
    return settingsManager->value("color/search", QColor(schemes[0].search)).value<QColor>();
}

QColor SettingsManager::error() {
    return settingsManager->value("color/error", QColor(schemes[0].error)).value<QColor>();
}

void SettingsManager::setForeground(QColor color) {
    settingsManager->setValue("color/foreground", color);
}

void SettingsManager::setBackground(QColor color) {
    settingsManager->setValue("color/background", color);
}

void SettingsManager::setKeywords(QColor color) {
    settingsManager->setValue("color/keywords", color);
}

void SettingsManager::setTypes(QColor color) {
    settingsManager->setValue("color/types", color);
}

void SettingsManager::setValues(QColor color) {
    settingsManager->setValue("color/values", color);
}

void SettingsManager::setComments(QColor color) {
    settingsManager->setValue("color/comments", color);
}

void SettingsManager::setFunctions(QColor color) {
    settingsManager->setValue("color/functions", color);
}

void SettingsManager::setTermForeground(QColor color) {
    settingsManager->setValue("color/termForeground", color);
}

void SettingsManager::setTermBackground(QColor color) {
    settingsManager->setValue("color/termBackground", color);
}

void SettingsManager::setLinesBackground(QColor color) {
    settingsManager->setValue("color/linesBackground", color);
}

void SettingsManager::setLinesForeground(QColor color) {
    settingsManager->setValue("color/linesForeground", color);
}

void SettingsManager::setSearch(QColor color) {
    settingsManager->setValue("color/search", color);
}

void SettingsManager::setError(QColor color) {
    settingsManager->setValue("color/error", color);
}
