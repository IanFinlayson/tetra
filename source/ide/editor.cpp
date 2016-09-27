/* editor.cpp
 * A text editor component that is designed specifically for Tetra code
 * */

#include <QtWidgets>

#include "editor.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"

Editor::Editor(QWidget* parent)
    : QPlainTextEdit(parent) {
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(updateCursorCoordinates()));
    connect(this, SIGNAL(blockCountChanged(int)), this,
            SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this,
            SLOT(updateLineNumberArea(QRect, int)));

    lineNumberArea = NULL;
    setPlainText("");
    highlighter = new Highlighter(document());

    ensureCursorVisible();
    setCenterOnScroll(false);
    fileName = "";

    updateSettings();

    copyAvailable = redoAvailable = undoAvailable = false;
    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(setCopyAvailable(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoAvailable(bool)));
    connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoAvailable(bool)));

    /* remove search and error highlights when text is changed */
    connect(this, SIGNAL(textChanged()), SLOT(unhighlightLine()));
}

void Editor::updateSettings() {
    /* if line numbers are on */
    if (SettingsManager::lineNo()) {
        /* create if needed */
        if (!lineNumberArea) {
            lineNumberArea = new LineNumberArea(this);
        }

        /* set them up */
        lineNumberArea->show();
        lineNumberArea->setFont(SettingsManager::font());
        updateLineNumberAreaWidth(0);
        lineNumberArea->setStyleSheet("background-color: " +
                                      SettingsManager::background().name() +
                                      ";"
                                      "color: " +
                                      SettingsManager::foreground().name() + ";");
    } else {
        /* delete them if needed */
        if (lineNumberArea) {
            delete lineNumberArea;
            lineNumberArea = NULL;
            setViewportMargins(0, 0, 0, 0);
        }
    }

    /* set the tab width */
    tabWidth = SettingsManager::tabWidth();

    /* set the font and colors */
    setFont(SettingsManager::font());
    setStyleSheet(
        "QPlainTextEdit {"
        "background-color: " +
        SettingsManager::background().name() +
        ";"
        "color: " +
        SettingsManager::foreground().name() +
        ";"
        "}");

    /* update syntax highlighting */
    delete highlighter;
    highlighter = new Highlighter(document());
}

void Editor::setUpConnections(MainWindow* parent) {
    this->parent = parent;
    connect(this, SIGNAL(copyAvailable(bool)), parent->ui->actionCopy,
            SLOT(setEnabled(bool)));
    connect(this, SIGNAL(copyAvailable(bool)), parent->ui->actionCut,
            SLOT(setEnabled(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), parent->ui->actionRedo,
            SLOT(setEnabled(bool)));
    connect(this, SIGNAL(undoAvailable(bool)), parent->ui->actionUndo,
            SLOT(setEnabled(bool)));
    connect(this->document(), SIGNAL(contentsChanged()), parent,
            SLOT(documentWasModified()));
    connect(this, SIGNAL(cursorPositionChanged()), parent,
            SLOT(updateCoordinates()));
    connect(this, SIGNAL(cursorPositionChanged()), parent,
            SLOT(updateCoordinates()));
}

void Editor::setCopyAvailable(bool is) {
    copyAvailable = is;
}

void Editor::setRedoAvailable(bool is) {
    redoAvailable = is;
}

void Editor::setUndoAvailable(bool is) {
    undoAvailable = is;
}

bool Editor::canCopy() {
    return copyAvailable;
}

bool Editor::canUndo() {
    return undoAvailable;
}

bool Editor::canRedo() {
    return redoAvailable;
}

/* save as - ask the user for a file name, save the file, and return
 * success/failure */
bool Editor::saveas() {
    fileName =
        QFileDialog::getSaveFileName(this, tr("Save File"), "", "Tetra (*.ttr)");
    if (fileName == "") {
        return false;
    } else {
        return save();
    }
}

/* save - if we have a name, update it, else ask the user and then save, return
 * success/failure */
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

/* infer the tab width of this file */
void Editor::inferTabWidth(QString fileText) {
    /* loop through all of the lines */
    QStringList lines = fileText.split('\n');
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);

        /* if this line starts with a space */
        if (line.length() >= 1 && line.at(0) == ' ') {
            /* count them */
            int spaces = 1;

            while (spaces < line.size() && line.at(spaces) == ' ') {
                spaces++;
            }

            /* set it and return, we are done */
            setTabWidth(spaces);
            return;
        }
    }

    /* if NO lines started with a space, then leave default */
    setTabWidth(SettingsManager::tabWidth());
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

        /* infer the tab width in this file */
        inferTabWidth(fileText);

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
    if (lineNumberArea) {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
}

void Editor::updateLineNumberArea(const QRect& rect, int dy) {
    if (!lineNumberArea) {
        return;
    }

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

    if (lineNumberArea) {
        QRect cr = contentsRect();
        lineNumberArea->setGeometry(
            QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    }
}

/* draw the line number area on the left */
void Editor::lineNumberAreaPaintEvent(QPaintEvent* event) {
    if (!lineNumberArea) {
        return;
    }

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), SettingsManager::linesBackground());

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(SettingsManager::linesForeground());
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void Editor::moveCursor(int line, int col) {
    int currentLineNumber = cursor.blockNumber() + 1;
    int moves = line - currentLineNumber;
    if (moves < 0) {
        cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, -moves);
    } else {
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, moves);
    }
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, col);
    setTextCursor(cursor);
}

/* highlight the current line red for an error message */
void Editor::errorHighlight() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(SettingsManager::error());
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

/* remove all line highlighting */
void Editor::unhighlightLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    setExtraSelections(extraSelections);
}

/* get and set the tab width for this file */
void Editor::setTabWidth(int tabWidth) {
    this->tabWidth = tabWidth;
}
int Editor::getTabWidth() {
    return this->tabWidth;
}

/* highlight a search term */
void Editor::highlightAll(QString term, bool matchCase) {
    Qt::CaseSensitivity cs = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;

    /* clear any existing ones */
    unhighlightLine();

    /* the list of selected highlighted words */
    QList<QTextEdit::ExtraSelection> extraSelections;

    /* for each line */
    QStringList lines = toPlainText().split('\n');
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);

        /* keep track of last match if any to highlight all */
        int last = -1;

        /* if this line matches */
        int col = line.indexOf(term, 0, cs);
        while (col > last) {
            /* add a highlight for it */
            moveCursor(i + 1, col);

            QTextEdit::ExtraSelection currentWord;
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                                term.size());

            currentWord.format.setBackground(SettingsManager::search());
            currentWord.cursor = cursor;
            extraSelections.append(currentWord);

            /* find next instance, if any */
            col = line.indexOf(term, col + 1, cs);
        }
    }

    /* add all of these */
    setExtraSelections(extraSelections);
}

/* perform a search with jumps and highlights */
bool Editor::searchDir(QString term,
                       bool forward,
                       bool matchCase,
                       bool highlight) {
    Qt::CaseSensitivity cs = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;

    bool found = false;

    /* remember cursor (highlight ruins it) */
    QTextCursor c = textCursor();

    /* do the highlighting of search terms */
    if (highlight) {
        highlightAll(term, matchCase);
    }

    /* try to search forwards */
    QString text = toPlainText();
    int pos;

    if (forward) {
        pos = text.indexOf(term, c.position() + 1, cs);
    } else {
        pos = text.lastIndexOf(term, c.position() - 1, cs);
    }

    if (pos != -1) {
        /* move to this position */
        c.setPosition(pos, QTextCursor::MoveAnchor);
        found = true;
    } else {
        /* wrap around */
        int pos;
        if (forward) {
            pos = text.indexOf(term, 0, cs);
        } else {
            pos = text.lastIndexOf(term, -1, cs);
        }
        if (pos != -1) {
            /* move to this position */
            c.setPosition(pos, QTextCursor::MoveAnchor);
            found = true;
        }
    }

    /* reset it at new place */
    setTextCursor(c);
    return found;
}

void Editor::replaceNext(QString before, QString after, bool matchCase) {
    /* search forward first */
    if (!searchDir(before, true, matchCase, false)) {
        return;
    }

    /* select the text given */
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                        before.size());
    cursor.insertText(after);
}

/* replace all text in the document */
void Editor::replaceAll(QString before, QString after, bool matchCase) {
    /* get the text */
    QString text = toPlainText();

    /* do the actual replacement */
    text.replace(before, after,
                 matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);

    /* set the text back
     * this is done using the cursor so as not to erase all history! */
    QTextCursor cursor(document());
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}
