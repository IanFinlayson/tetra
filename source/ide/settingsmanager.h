/* settingsmanager.h
 * code to run or debug a program */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QFont>

class SettingsManager {
    public:
        static void init();

        /* get the current values */
        static int tabWidth();
        static bool lineNo();
        static bool smartEdit();
        static QFont font();

        /* set new values */
        static void setTabWidth(int);
        static void setLineNo(bool);
        static void setSmartEdit(bool);
        static void setFont(QFont);

    private:
        static QSettings* qset;
};

#endif

