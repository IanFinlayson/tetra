#include "debugwindow.h"
#include <QtWidgets>
/*Debug Windows are created during the debugging of a Tetra program and
 * represent single threads in the program*/
DebugWindow::DebugWindow(QWidget *parent) : QPlainTextEdit(parent) {
  debuggerLineNumberArea = new DebuggerLineNumberArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this,
          SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect, int)), this,
          SLOT(updateLineNumberArea(QRect, int)));

  updateLineNumberAreaWidth(0);

  lineNumbersVisible = true;
  lineHighlighted = false;
  tabWidth = 4;
  this->setReadOnly(true);
}

int DebugWindow::lineNumberAreaWidth() {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

  return space;
}

void DebugWindow::updateLineNumberAreaWidth(int /* newBlockCount */) {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void DebugWindow::updateLineNumberArea(const QRect &rect, int dy) {
  if (dy)
    debuggerLineNumberArea->scroll(0, dy);
  else
    debuggerLineNumberArea->update(0, rect.y(), debuggerLineNumberArea->width(),
                                   rect.height());

  if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}

void DebugWindow::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  debuggerLineNumberArea->setGeometry(
      QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void DebugWindow::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(debuggerLineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int)blockBoundingRect(block).height();

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, debuggerLineNumberArea->width(),
                       fontMetrics().height(), Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + (int)blockBoundingRect(block).height();
    ++blockNumber;
  }
}

void DebugWindow::showLineNumbers(bool arg1) {
  if (arg1 == true) {
    this->debuggerLineNumberArea->show();
  } else {
    this->debuggerLineNumberArea->hide();
  }
}

void DebugWindow::moveCursor(int lineNumberToHighlight) {
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
void DebugWindow::highlightLine(QColor color) {
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

void DebugWindow::unhighlightLine() {
  QList<QTextEdit::ExtraSelection> extraSelections;
  setExtraSelections(extraSelections);
  lineHighlighted = false;
}

bool DebugWindow::checkLineHighlighted() { return lineHighlighted; }

void DebugWindow::setTabWidth(int tabWidth) { this->tabWidth = tabWidth; }

int DebugWindow::getTabWidth() { return this->tabWidth; }

// fills DebugWindow with text
void DebugWindow::fill(QString text) { this->setPlainText(text); }
