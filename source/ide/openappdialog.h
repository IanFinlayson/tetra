#ifndef OPENAPPDIALOG_H
#define OPENAPPDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QPushButton>

namespace Ui {
class OpenAppDialog;
}

class OpenAppDialog : public QDialog
{
    Q_OBJECT

protected:
    virtual void keyPressEvent(QKeyEvent *e);

public:
    explicit OpenAppDialog(QWidget *parent = 0);
    ~OpenAppDialog();
    void setMainWindow(MainWindow*);

private slots:
    void on_quitAppButton_clicked();
    void on_newProjButton_clicked();
    void on_openProjButton_clicked();

private:
    Ui::OpenAppDialog *ui;
    MainWindow *mainwindow;
};


#endif // OPENAPPDIALOG_H
