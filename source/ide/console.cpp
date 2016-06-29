/* console.cpp
 * A text editor component that is designed specifically for Tetra code
 * */

#include <QtWidgets>

#include "console.h"
#include "ui_mainwindow.h"
#include "settingsmanager.h"

Console::Console(MainWindow* parent) : QPlainTextEdit(parent), VirtualConsole() {
    setContentsMargins(50, 50, 50, 50);

    ensureCursorVisible();
    setCenterOnScroll(false);;
    setReadOnly(true);
    this->parent = parent;

    updateSettings();
}

void Console::setUpConnections(MainWindow* parent) {
    this->parent = parent;
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
}

/* TODO change this to use the actual console itself the difficulty in that is
 * that we can't return right away like we must for the interface... */
std::string Console::receiveStandardInput() {
    while (true) {
        bool ok;
        QString text = QInputDialog::getText(parent, "Enter Input", "Enter Input", QLineEdit::Normal, "", &ok);
        if (ok) {
            return text.toStdString();
        }
    }
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



