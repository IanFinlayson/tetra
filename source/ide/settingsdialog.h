/* settingsdialog.h
 * declares the SettingsDialog class which allows the user to modify IDE
 * settings */

#include <QDialog>
#include <QSettings>
#include "ui_settings.h"

/* a simple struct for storing the color schemes */
const int NUM_SCHEMES = 12;
struct ColorScheme {
    const char* name;
    unsigned int foreground;
    unsigned int background;
    unsigned int keywords;
    unsigned int types;
    unsigned int values;
    unsigned int comments;
    unsigned int functions;
    unsigned int termForeground;
    unsigned int termBackground;
    unsigned int linesForeground;
    unsigned int linesBackground;
    unsigned int search;
    unsigned int error;
};

class SettingsDialog : public QDialog, public Ui::Settings {
    Q_OBJECT

  private:
    QSettings* settings_manager;
    QFont font_choice;

  private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
    void askFont();
    void askColor();
    void selectTheme(int index);

  public:
    SettingsDialog(QWidget* parent);
};
