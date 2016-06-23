/* console.cpp
 * A text editor component that is designed specifically for Tetra code
 * */

#include <QtWidgets>

#include "console.h"
#include "ui_mainwindow.h"
#include "settingsmanager.h"

Console::Console(MainWindow* parent) : QPlainTextEdit(parent), VirtualConsole() {
    setContentsMargins(50, 50, 50, 50);

    /* set the background color */
    setStyleSheet("background-color:#404244; color: #eeeeee;");

    /* set to a monospaced font */
    QFont font = SettingsManager::font();
    setFont(font);

    ensureCursorVisible();
    setCenterOnScroll(false);;
    setReadOnly(true);
}

void Console::setUpConnections(MainWindow* parent) {
    this->parent = parent;
}

std::string Console::receiveStandardInput() {
    /* make it writeable */
    setReadOnly(false);

    setFocus(Qt::OtherFocusReason);

    return "42";
}

void Console::processStandardOutput(const std::string& text) {
    QTextCursor* cursor = new QTextCursor(document());
    cursor->movePosition(QTextCursor::End);
    cursor->insertText(QString(text.c_str()));
}

/* when keys are pressed in the widget */
void Console::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Right) {
        int* seven = NULL;
        *seven = 7;
    }

    /* else, just pass it */
    else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

void Console::resizeEvent(QResizeEvent* e) {
    QPlainTextEdit::resizeEvent(e);
}



