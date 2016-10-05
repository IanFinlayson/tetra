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
    void errorHighlight();

    /* updat the editor based on the settings from the settings manager */
    void updateSettings();

    /* the editor must know the indent width for doing smart editing */
    void setTabWidth(int width);
    int getTabWidth();
    void inferTabWidth(QString fileText);

    /* functions to save and open files */
    bool save();
    bool saveas();
    bool open(QString fname);

    /* returns whether the editor is empty and not unsaved */
    bool isEmpty();

    /* returns the open file name of this editor, or "" if none set yet */
    QString getOpenFile();

    /* move the cursors and get the coordinates of it */
    void moveCursor(int line, int col = 0);
    QString getCoordinates();

    /* functions for the main window to just ask if these are available */
    bool canCopy();
    bool canUndo();
    bool canRedo();

    /* implement searching with highlights and jumps */
    void highlightAll(QString term, bool matchCase);
    bool searchDir(QString term, bool forward, bool matchCase, bool highlight);

    /* implement replacing */
    void replaceNext(QString before, QString after, bool matchCase);
    void replaceAll(QString before, QString after, bool matchCase);

  public slots:
    /* update aspects of the GUI */
    void updateCursorCoordinates();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect&, int);
    void unhighlightLine();

    /* copy only available when something is selected. undo/redo only available
     * when there is a history */
    void setCopyAvailable(bool);
    void setRedoAvailable(bool);
    void setUndoAvailable(bool);

  protected:
    virtual void keyPressEvent(QKeyEvent* e);
    void resizeEvent(QResizeEvent* event);

  private:
    /* these are for inferring the tab amount from loaded files */
    int getLeadingSpaces();
    bool isTab(QString direction);

    QString fileName;
    QTextCursor cursor;
    QString coordinates;
    QWidget* lineNumberArea;
    int tabWidth;
    Highlighter* highlighter;
    MainWindow* parent;
    bool copyAvailable, redoAvailable, undoAvailable;
};

/* the area on the left of the screen showing line numbers */
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
