#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include "mainwindow.h"
class Editor: public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *e);

public slots:

private:
    int getLeadingSpaces(QString);

};

#endif // EDITOR_H
