#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QPlainTextEdit>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class DebugWindow: public QPlainTextEdit
{
    Q_OBJECT
public:
    DebugWindow(QWidget *parent = 0);

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
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QTextCursor cursor;
    QWidget *debuggerLineNumberArea;
    bool lineNumbersVisible;
    bool lineHighlighted;
    int tabWidth;
    void fill(QString);
};

class DebuggerLineNumberArea : public QWidget{
public:
    DebuggerLineNumberArea(DebugWindow *debugWindow) : QWidget(debugWindow) {
        debugger = debugWindow;
    }

    QSize sizeHint() const {
        return QSize(debugger->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        debugger->lineNumberAreaPaintEvent(event);
    }

private:
    DebugWindow *debugger;
};
#endif
