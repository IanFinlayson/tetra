/********************************************************************************
** Form generated from reading UI file 'openappdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENAPPDIALOG_H
#define UI_OPENAPPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenAppDialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *newProjButton;
    QSpacerItem *verticalSpacer;
    QPushButton *openProjButton;
    QSpacerItem *verticalSpacer_2;
    QPushButton *quitAppButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QLabel *logo;

    void setupUi(QDialog *OpenAppDialog)
    {
        if (OpenAppDialog->objectName().isEmpty())
            OpenAppDialog->setObjectName(QStringLiteral("OpenAppDialog"));
        OpenAppDialog->resize(625, 325);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OpenAppDialog->sizePolicy().hasHeightForWidth());
        OpenAppDialog->setSizePolicy(sizePolicy);
        OpenAppDialog->setStyleSheet(QLatin1String("background: rgb(255, 255, 255)\n"
"\n"
""));
        gridLayoutWidget = new QWidget(OpenAppDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 30, 631, 271));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        newProjButton = new QPushButton(gridLayoutWidget);
        newProjButton->setObjectName(QStringLiteral("newProjButton"));
        newProjButton->setCursor(QCursor(Qt::ArrowCursor));
        newProjButton->setAcceptDrops(false);
        newProjButton->setAutoFillBackground(false);
        newProjButton->setStyleSheet(QLatin1String("QPushButton {\n"
"color: black;\n"
"padding: 5px;\n"
"border: 2px solid #555;\n"
"border-radius: 11px;\n"
"background: qconicalgradient(cx:0, cy:1, angle:109, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"min-width: 120px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #bbb);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"background: qradialgradient(cx: 0.4, cy: -0.1,\n"
"fx: 0.4, fy: -0.1,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #ddd);\n"
"}"));

        verticalLayout->addWidget(newProjButton);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        openProjButton = new QPushButton(gridLayoutWidget);
        openProjButton->setObjectName(QStringLiteral("openProjButton"));
        openProjButton->setCursor(QCursor(Qt::ArrowCursor));
        openProjButton->setAcceptDrops(false);
        openProjButton->setStyleSheet(QLatin1String("QPushButton {\n"
"color: black;\n"
"padding: 5px;\n"
"border: 2px solid #555;\n"
"border-radius: 11px;\n"
"background: qconicalgradient(cx:0, cy:1, angle:109, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"min-width: 120px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #bbb);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"background: qradialgradient(cx: 0.4, cy: -0.1,\n"
"fx: 0.4, fy: -0.1,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #ddd);\n"
"}"));

        verticalLayout->addWidget(openProjButton);

        verticalSpacer_2 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        quitAppButton = new QPushButton(gridLayoutWidget);
        quitAppButton->setObjectName(QStringLiteral("quitAppButton"));
        quitAppButton->setCursor(QCursor(Qt::ArrowCursor));
        quitAppButton->setAcceptDrops(false);
        quitAppButton->setStyleSheet(QLatin1String("QPushButton {\n"
"color: black;\n"
"padding: 5px;\n"
"border: 2px solid #555;\n"
"border-radius: 11px;\n"
"background: qconicalgradient(cx:0, cy:1, angle:109, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"min-width: 120px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #bbb);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"background: qradialgradient(cx: 0.4, cy: -0.1,\n"
"fx: 0.4, fy: -0.1,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #ddd);\n"
"}"));

        verticalLayout->addWidget(quitAppButton);


        gridLayout->addLayout(verticalLayout, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        logo = new QLabel(gridLayoutWidget);
        logo->setObjectName(QStringLiteral("logo"));
        logo->setEnabled(true);
        logo->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(logo, 0, 0, 1, 3);


        retranslateUi(OpenAppDialog);

        QMetaObject::connectSlotsByName(OpenAppDialog);
    } // setupUi

    void retranslateUi(QDialog *OpenAppDialog)
    {
        OpenAppDialog->setWindowTitle(QApplication::translate("OpenAppDialog", "Dialog", 0));
        newProjButton->setText(QApplication::translate("OpenAppDialog", "New Project", 0));
        openProjButton->setText(QApplication::translate("OpenAppDialog", "Open Project", 0));
        quitAppButton->setText(QApplication::translate("OpenAppDialog", "Quit", 0));
        logo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OpenAppDialog: public Ui_OpenAppDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENAPPDIALOG_H
