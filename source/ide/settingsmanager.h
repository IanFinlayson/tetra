/* settingsmanager.h
 * code to run or debug a program */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QFont>

class SettingsManager {
    public:
        static void init();

        static int tabWidth();
        static bool lineNo();
        static bool smartEdit();
        static QFont font();


    private:
        static QSettings* qset;
};

#endif

