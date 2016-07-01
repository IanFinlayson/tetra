/* console.h
 * a cusomt console for running tetra code */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QPlainTextEdit>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE


class Console : public QPlainTextEdit {
    Q_OBJECT
  public:
    Console(MainWindow* mainWindow = NULL);

    /* sets the link to the parent window and connections between them */
    void setUpConnections(MainWindow* parent);

    /* update the settings */
    void updateSettings();

    /* for I/O */
    void write(QString text);

  signals:
    void enterPressed(QString);

  protected:
    virtual void keyPressEvent(QKeyEvent* e);
    void resizeEvent(QResizeEvent* event);

  private:
    MainWindow* parent;
};
#endif

