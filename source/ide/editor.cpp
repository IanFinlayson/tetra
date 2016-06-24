/* editor.cpp
 * A text editor component that is designed specifically for Tetra code
 * */

#include <QtWidgets>

#include "settingsmanager.h"
#include "editor.h"
#include "ui_mainwindow.h"

Editor::Editor(QWidget* parent) : QPlainTextEdit(parent) {
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursorCoordinates()));
    lineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));

    updateLineNumberAreaWidth(0);
    setPlainText("");
    highlighter = new Highlighter(document());

    ensureCursorVisible();
    setCenterOnScroll(false);
    fileName = "";

    updateSettings();
}


void Editor::updateSettings() {
    /* set line number and tab width */
    if (SettingsManager::lineNo()) {
        lineNumberArea->show();
    } else {
        lineNumberArea->hide();
    }
    tabWidth = SettingsManager::tabWidth();

    /* set the font */
    setFont(SettingsManager::font());



    /* TODO should be part of color scheme */
    setStyleSheet("background-color:#f5f5f5;");
}

void Editor::setUpConnections(MainWindow* parent) {
    this->parent = parent;
    connect(this, SIGNAL(copyAvailable(bool)), parent->ui->actionCopy, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(copyAvailable(bool)), parent->ui->actionCut, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), parent->ui->actionRedo, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(undoAvailable(bool)), parent->ui->actionUndo, SLOT(setEnabled(bool)));
    connect(this->document(), SIGNAL(contentsChanged()), parent, SLOT(documentWasModified()));
    connect(this, SIGNAL(cursorPositionChanged()), parent, SLOT(updateCoordinates()));
    connect(this, SIGNAL(cursorPositionChanged()), parent, SLOT(updateCoordinates()));
}


/* save as - ask the user for a file name, save the file, and return success/failure */
bool Editor::saveas() {
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", "Tetra (*.ttr)");
    if (fileName == "") {
        return false;
    } else {
        return save();
    }
}

/* save - if we have a name, update it, else ask the user and then save, return success/failure */
bool Editor::save() {
    if (fileName == "") {
        return saveas();
    } else {
        QFile ttrFile(fileName);
        if (!ttrFile.open(QFile::WriteOnly | QFile::Text)) {
            return false;
        }

        QTextStream out(&ttrFile); 
        out << toPlainText();
        ttrFile.flush();
        ttrFile.close();
        document()->setModified(false);
        return true;
    }
}

/* open a file and return success or failure */
bool Editor::open(QString fname) {
    if (fname.isEmpty()) {
        return false;
    }

    QFile ttrFile(fname);
    if (ttrFile.open(QFile::ReadOnly | QFile::Text)) {
        fileName = fname;
        QTextStream in(&ttrFile);
        QString fileText = in.readAll();
        ttrFile.close();
        setPlainText(fileText);
        return true;
    } 
    
    return false;
}

/* return whether the editor is empty and not unsaved */
bool Editor::isEmpty() {
    if (document()->isModified() || !document()->isEmpty()) {
        return false;
    } else {
        return true;
    }
}


/* return the open file name (will be "" if none set) */
QString Editor::getOpenFile() {
    return fileName;
}

/* overrides default navigation for smart editing */
void Editor::keyPressEvent(QKeyEvent* e) {

    /* if we're not doing smart editing, just pass it up */
    if (!SettingsManager::smartEdit()) {
        QPlainTextEdit::keyPressEvent(e);
        return;
    }

    cursor = this->textCursor();

    /* replaces tab key with predetermined amount of spaces */
    if (e->key() == Qt::Key_Tab) {
        int pos = cursor.positionInBlock();
        if ((pos + tabWidth) % tabWidth == 0 || getLeadingSpaces() < pos) {
            for (int i = 0; i < tabWidth; i++) {
                cursor.insertText(" ");
            }
        } else {
            while (pos > tabWidth) {
                pos -= tabWidth;
            }
            for (int i = 0; i < pos; i++) {
                cursor.deletePreviousChar();
            }
            for (int i = 0; i < tabWidth; i++) {
                cursor.insertText(" ");
            }
        }

        /* when enter key is pressed, auto indents new line */
    } else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        int leadingSpaces = getLeadingSpaces();
        if (cursor.block().text().endsWith(":")) {
            leadingSpaces += tabWidth;
        }
        cursor.insertText("\n");
        for (int i = leadingSpaces; i >= tabWidth; i -= tabWidth) {
            for (int i = 0; i < tabWidth; i++) {
                cursor.insertText(" ");
            }
        }
    }

    /* smart backspace */
    else if (e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete) {
        if (isTab("left")) {
            int pos = cursor.positionInBlock();
            while (pos > tabWidth) {
                pos -= tabWidth;
            }
            for (int i = 0; i < pos; i++) {
                QPlainTextEdit::keyPressEvent(e);
            }
        } else {
            QPlainTextEdit::keyPressEvent(e);
        }
    }

    /* smart navigate */
    else if (e->key() == Qt::Key_Left) {
        if (isTab("left")) {
            int pos = cursor.positionInBlock();
            while (pos > tabWidth) {
                pos -= tabWidth;
            }
            for (int i = 0; i < pos; i++) {
                QPlainTextEdit::keyPressEvent(e);
            }
        } else {
            QPlainTextEdit::keyPressEvent(e);
        }
    } else if (e->key() == Qt::Key_Right) {
        if (isTab("right")) {
            for (int i = 0; i < tabWidth; i++) {
                QPlainTextEdit::keyPressEvent(e);
            }
        } else {
            QPlainTextEdit::keyPressEvent(e);
        }
    }

    /* else, just pass it */
    else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

/* return the number of leading spaces on the line with the cursor */
int Editor::getLeadingSpaces() {
    cursor = this->textCursor();
    QString line = cursor.block().text();
    int spaces = 0;
    while (line[spaces].isSpace()) {
        spaces++;
    }
    return spaces;
}

bool Editor::isTab(QString direction) {
    cursor = this->textCursor();
    int pos = cursor.positionInBlock();
    QString line = cursor.block().text();
    QString substring;
    bool isTab = false;
    if (direction == "left") {
        substring = line.mid(pos - tabWidth, tabWidth);
    } else {
        substring = line.mid(pos, tabWidth);
    }
    QString tab(tabWidth, ' ');
    if (substring == tab) {
        isTab = true;
    }
    return isTab;
}

/* update the cursor coordinates */
void Editor::updateCursorCoordinates() {
    cursor = this->textCursor();
    QString x = QString::number(cursor.blockNumber() + 1);
    QString y = QString::number(cursor.positionInBlock());
    coordinates = x + ", " + y;
}

/* return the coordinates of the cursor */
QString Editor::getCoordinates() {
    return coordinates;
}

int Editor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void Editor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::updateLineNumberArea(const QRect& rect, int dy) {
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void Editor::resizeEvent(QResizeEvent* e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/* draw the line number area on the left */
void Editor::lineNumberAreaPaintEvent(QPaintEvent* event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(230, 230, 230));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top =
        (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void Editor::moveCursor(int lineNumberToHighlight) {
    int currentLineNumber = cursor.blockNumber() + 1;
    int moves = lineNumberToHighlight - currentLineNumber;
    if (moves) {
        if (moves < 0) {
            cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, -moves);
        } else {
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, moves);
        }
        cursor.movePosition(QTextCursor::StartOfLine);
        setTextCursor(cursor);
    }
}

void Editor::highlightLine(QColor color) {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = color.lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void Editor::unhighlightLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    setExtraSelections(extraSelections);
}

void Editor::setTabWidth(int tabWidth) {
    this->tabWidth = tabWidth;
}

int Editor::getTabWidth() {
    return this->tabWidth;
}
