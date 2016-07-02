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

/* the widget which displays line numbers on the left */
class LineNumberArea;

class Editor : public QPlainTextEdit {
    Q_OBJECT
  public:
    Editor(QWidget* parent = NULL);

    /* sets the link to the parent window and connections between them */
    void setUpConnections(MainWindow* parent);

    /* functions for managind the line number widget */
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

    /* allows the highlighting of lines for reporting errors and warnings */
    void highlightLine(QColor);
    bool checkLineHighlighted();

    /* updat the editor based on the settings from the settings manager */
    void updateSettings();

    /* the editor must know the indent width for doing smart editing */
    void setTabWidth(int width);
    int getTabWidth();
    
    /* functions to save and open files */
    bool save();
    bool saveas();
    bool open(QString fname);

    /* returns whether the editor is empty and not unsaved */
    bool isEmpty();

    /* returns the open file name of this editor, or "" if none set yet */
    QString getOpenFile(); 

    /* move the cursors and get the coordinates of it */
    void moveCursor(int);
    QString getCoordinates();

    /* functions for the main window to just ask if these are available */
    bool canCopy();
    bool canUndo();
    bool canRedo();

  private slots:
    void updateCursorCoordinates();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect&, int);
    void unhighlightLine();

    void setCopyAvail(bool);
    void setRedoAvail(bool);
    void setUndoAvail(bool);

  protected:
    virtual void keyPressEvent(QKeyEvent* e);
    void resizeEvent(QResizeEvent* event);

  private:
    int getLeadingSpaces();
    bool isTab(QString direction);

    QString fileName;
    QTextCursor cursor;
    QString coordinates;
    QWidget* lineNumberArea;
    int tabWidth;
    Highlighter* highlighter;
    MainWindow* parent;
    bool copyAvail, redoAvail, undoAvail;
    bool lineHighlighted;
};

class LineNumberArea : public QWidget {
  public:
    LineNumberArea(Editor* editor)
        : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

  protected:
    void paintEvent(QPaintEvent* event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

  private:
    Editor* codeEditor;
};
#endif
