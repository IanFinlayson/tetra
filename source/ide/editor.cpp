/* editor.cpp
 * A text editor component that is designed specifically for Tetra code
 * */

#include "editor.h"
#include <QtWidgets>

Editor::Editor(QWidget *parent) : QPlainTextEdit(parent) {
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursorCoordinates()));

  lineNumberArea = new LineNumberArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));

  updateLineNumberAreaWidth(0);

  /* set the background color
   * TODO: make it come from a color scheme instead */
  setStyleSheet("background-color:#f5f5f5;");


  lineNumbersVisible = true;
  lineHighlighted = false;
  tabWidth = 4;
}

// overrides default navigation
void Editor::keyPressEvent(QKeyEvent *e) {
  cursor = this->textCursor();
  if (e->key() == Qt::Key_Tab) {
    // replaces tab key with predetermined amount of spaces
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
  } else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
    // when enter key is pressed, auto indents new line
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
  } else if (e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete) {
    //"smart" backspace
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
  } else if (e->key() == Qt::Key_Left) {
    //"smart" navigate
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
  } else {
    QPlainTextEdit::keyPressEvent(e);
  }
}

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

void Editor::updateCursorCoordinates() {
  cursor = this->textCursor();
  QString x = QString::number(cursor.blockNumber() + 1);
  QString y = QString::number(cursor.positionInBlock());
  coordinates = x + ", " + y;
}

QString Editor::getCoordinates() { return coordinates; }

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

void Editor::updateLineNumberArea(const QRect &rect, int dy) {
  if (dy) {
    lineNumberArea->scroll(0, dy);
  } else {
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect())) {
    updateLineNumberAreaWidth(0);
  }
}

void Editor::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int)blockBoundingRect(block).height();

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                       Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + (int)blockBoundingRect(block).height();
    ++blockNumber;
  }
}

void Editor::showLineNumbers(bool arg1) {
  if (arg1 == true) {
    this->lineNumberArea->show();
  } else {
    this->lineNumberArea->hide();
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
  lineHighlighted = true;
}

void Editor::unhighlightLine() {
  QList<QTextEdit::ExtraSelection> extraSelections;
  setExtraSelections(extraSelections);
  lineHighlighted = false;
}

bool Editor::checkLineHighlighted() {
  return lineHighlighted;
}

void Editor::setTabWidth(int tabWidth) {
  this->tabWidth = tabWidth;
}

int Editor::getTabWidth() {
  return this->tabWidth;
}


