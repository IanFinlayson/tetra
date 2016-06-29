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

        static QColor foreground();
        static QColor background();
        static QColor keywords();
        static QColor types();
        static QColor values();
        static QColor comments();
        static QColor functions();
        static QColor termForeground();
        static QColor termBackground();
        static QColor linesForeground();
        static QColor linesBackground();

        /* set new values */
        static void setTabWidth(int);
        static void setLineNo(bool);
        static void setSmartEdit(bool);
        static void setFont(QFont);

        static void setForeground(QColor);
        static void setBackground(QColor);
        static void setKeywords(QColor);
        static void setTypes(QColor);
        static void setValues(QColor);
        static void setComments(QColor);
        static void setFunctions(QColor);
        static void setTermForeground(QColor);
        static void setTermBackground(QColor);
        static void setLinesForeground(QColor);
        static void setLinesBackground(QColor);

    private:
        static QSettings* qset;

};

#endif

