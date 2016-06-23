#include <QDialog>
#include <QSettings>
#include "ui_settings.h"

class SettingsDialog : public QDialog, public Ui::Settings {
    Q_OBJECT

    private:
        QSettings* settings_manager;

    private slots:
        void on_buttonBox_clicked(QAbstractButton* button);

    public:
        SettingsDialog(QWidget* parent);
};

