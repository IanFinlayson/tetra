/* editor.h
 * a cusomt text editor component for Tetra code */

#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class Editor : public QPlainTextEdit {
  Q_OBJECT
 public:
  Editor(QWidget *parent = 0);
  QString getCoordinates();

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();
  void showLineNumbers(bool);
  void moveCursor(int);
  void highlightLine(QColor);
  void unhighlightLine();
  bool checkLineHighlighted();
  void setTabWidth(int width);
  int getTabWidth();

 private slots:
  void updateCursorCoordinates();
  void updateLineNumberAreaWidth(int newBlockCount);
  void updateLineNumberArea(const QRect &, int);

 protected:
  virtual void keyPressEvent(QKeyEvent *e);
  void resizeEvent(QResizeEvent *event);

 private:
  int getLeadingSpaces();
  bool isTab(QString direction);
  QTextCursor cursor;
  QString coordinates;
  QWidget *lineNumberArea;
  bool lineNumbersVisible;
  bool lineHighlighted;
  int tabWidth;
};

class LineNumberArea : public QWidget {
 public:
  LineNumberArea(Editor *editor) : QWidget(editor) { codeEditor = editor; }

  QSize sizeHint() const { return QSize(codeEditor->lineNumberAreaWidth(), 0); }

 protected:
  void paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
  }

 private:
  Editor *codeEditor;
};
#endif
