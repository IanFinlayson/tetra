/* searchbox.h
 * custom line edit which only exists to make escape close the box */

#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QLineEdit>
#include <QObject>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class SearchBox : public QLineEdit {
    Q_OBJECT
  public:
    SearchBox(QWidget* mainWindow = NULL);

  signals:
    void closeSearch();

  protected:
    virtual void keyPressEvent(QKeyEvent* e);
};
#endif
