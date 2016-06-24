#include <QDialog>
#include <QSettings>
#include "ui_settings.h"

class SettingsDialog : public QDialog, public Ui::Settings {
    Q_OBJECT

    private:
        QSettings* settings_manager;
        QFont font_choice;

    private slots:
        void on_buttonBox_clicked(QAbstractButton* button);
        void askFont();

    public:
        SettingsDialog(QWidget* parent);
};

