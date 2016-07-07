
#ifndef REPLACE_DIALOG
#define REPLACE_DIALOG

#include <QDialog>
#include "ui_replace.h"

class ReplaceDialog : public QDialog, public Ui::Replace {
    Q_OBJECT

    public:
        ReplaceDialog(QWidget* parent);

    public slots:
        void replace();
        void replaceAll();
        void cancel();
        void next();

    private:
        Ui::Replace* ui;
};

#endif

