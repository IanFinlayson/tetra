/* console.cpp
 * A text editor component that is designed specifically for Tetra code
 * */

#include <QtWidgets>

#include "console.h"
#include "ui_mainwindow.h"
#include "settingsmanager.h"

Console::Console(MainWindow* parent) : QPlainTextEdit(parent) {
    setContentsMargins(50, 50, 50, 50);

    ensureCursorVisible();
    setCenterOnScroll(false);;
    setReadOnly(true);
    this->parent = parent;

    /* update all settings for the console */
    updateSettings();
}

void Console::setUpConnections(MainWindow* parent) {
    this->parent = parent;

    /* set it up so that console can pass input to main window */
    connect(this, SIGNAL(enterPressed(QString)), parent, SLOT(receiveInput(QString)));
}

void Console::updateSettings() {
    /* set colors */
    setStyleSheet("QPlainTextEdit {"
            "background-color: " + SettingsManager::termBackground().name() + ";"
            "color: " + SettingsManager::termForeground().name() + ";"
            "}");

    /* set font */
    QFont font = SettingsManager::font();
    setFont(font);

    /* set the cursor to be a block */
    QFontMetrics fm(font);
    setCursorWidth( fm.averageCharWidth() );
}
/* when keys are pressed in the widget */
void Console::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Right) {
        int* seven = NULL;
        *seven = 7;
    }

    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        emit enterPressed("7");
    }

    /* else, just pass it */
    else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

void Console::resizeEvent(QResizeEvent* e) {
    QPlainTextEdit::resizeEvent(e);
}


void Console::write(QString text) {
    QTextCursor* cursor = new QTextCursor(document());
    cursor->movePosition(QTextCursor::End);
    cursor->insertText(text);
}


