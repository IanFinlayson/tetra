#include <QDialog>
#include "ui_settings.h"

class SettingsDialog : public QDialog, public Ui::Settings {
    Q_OBJECT

public:
    SettingsDialog( QWidget * parent = 0);
};

