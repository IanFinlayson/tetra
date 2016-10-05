/* replacedialog.h
 * declares the ReplaceDialog class which allows for search and replace
 * functionality */

#ifndef REPLACE_DIALOG
#define REPLACE_DIALOG

#include <QDialog>
#include "ui_replace.h"

class MainWindow;

class ReplaceDialog : public QDialog, public Ui::Replace {
    Q_OBJECT

  public:
    ReplaceDialog(QWidget* parent);
    void updateSettings();

  public slots:
    void replace();
    void replaceAll();
    void cancel();
    void next();
    void saveMatchCase(int);

  private:
    Ui::Replace* ui;
    MainWindow* mainWindow;
};

#endif
