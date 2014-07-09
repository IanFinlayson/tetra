#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QLabel>
#include "mainwindow.h"
class Editor: public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = 0);
    QString getCoordinates();

private slots:
    void updateCursorCoordinates();


protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    int getLeadingSpaces();
    bool isTab(QString direction);
    QTextCursor cursor;
    QString coordinates;
};

#endif // EDITOR_H
