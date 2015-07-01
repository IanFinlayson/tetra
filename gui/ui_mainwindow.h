/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "editor.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionPrint;
    QAction *actionAbout_Tetra;
    QAction *actionDocumentation;
    QAction *actionCut;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSave_As;
    QAction *actionRun;
    QAction *actionDebug;
    QAction *actionOpen_Recent;
    QAction *actionDelete;
    QAction *actionSelect_All;
    QAction *actionBuild;
    QAction *actionStop;
    QAction *actionFind;
    QAction *actionMinimize;
    QAction *actionFull_Screen;
    QAction *actionLine_Numbers;
    QAction *actionClear_Output;
    QAction *actionResume;
    QAction *actionPause;
    QAction *actionStep_Through;
    QAction *actionStep_Out;
    QAction *actionStep_Over;
    QAction *actionExit_Debug_Mode;
    QAction *actionTab_Width;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    Editor *input;
    QLabel *tetraFileLabel;
    QLabel *cursorPosition;
    QMdiArea *threadMdi;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *outputLabel;
    QTextEdit *output;
    QPlainTextEdit *userInput;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuBuild;
    QMenu *menuDebug;
    QMenu *menuWindow;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(868, 597);
        MainWindow->setStyleSheet(QStringLiteral(""));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon);
        actionSave->setIconVisibleInMenu(false);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.printer.text.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon1);
        actionPrint->setIconVisibleInMenu(false);
        actionAbout_Tetra = new QAction(MainWindow);
        actionAbout_Tetra->setObjectName(QStringLiteral("actionAbout_Tetra"));
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName(QStringLiteral("actionDocumentation"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon2);
        actionUndo->setIconVisibleInMenu(false);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon3);
        actionRedo->setIconVisibleInMenu(false);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        actionRun->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.control.play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon4);
        actionRun->setIconVisibleInMenu(false);
        actionDebug = new QAction(MainWindow);
        actionDebug->setObjectName(QStringLiteral("actionDebug"));
        actionDebug->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.bug.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDebug->setIcon(icon5);
        actionDebug->setIconVisibleInMenu(false);
        actionOpen_Recent = new QAction(MainWindow);
        actionOpen_Recent->setObjectName(QStringLiteral("actionOpen_Recent"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QStringLiteral("actionSelect_All"));
        actionBuild = new QAction(MainWindow);
        actionBuild->setObjectName(QStringLiteral("actionBuild"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.protractor.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBuild->setIcon(icon6);
        actionBuild->setIconVisibleInMenu(false);
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.control.stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon7);
        actionStop->setIconVisibleInMenu(false);
        actionFind = new QAction(MainWindow);
        actionFind->setObjectName(QStringLiteral("actionFind"));
        actionMinimize = new QAction(MainWindow);
        actionMinimize->setObjectName(QStringLiteral("actionMinimize"));
        actionFull_Screen = new QAction(MainWindow);
        actionFull_Screen->setObjectName(QStringLiteral("actionFull_Screen"));
        actionLine_Numbers = new QAction(MainWindow);
        actionLine_Numbers->setObjectName(QStringLiteral("actionLine_Numbers"));
        actionLine_Numbers->setCheckable(true);
        actionLine_Numbers->setChecked(true);
        actionLine_Numbers->setEnabled(true);
        actionClear_Output = new QAction(MainWindow);
        actionClear_Output->setObjectName(QStringLiteral("actionClear_Output"));
        actionResume = new QAction(MainWindow);
        actionResume->setObjectName(QStringLiteral("actionResume"));
        actionResume->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.control.resume.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionResume->setIcon(icon8);
        actionPause = new QAction(MainWindow);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        actionPause->setEnabled(false);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.control.pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon9);
        actionStep_Through = new QAction(MainWindow);
        actionStep_Through->setObjectName(QStringLiteral("actionStep_Through"));
        actionStep_Through->setEnabled(false);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.debug.step.into.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStep_Through->setIcon(icon10);
        actionStep_Out = new QAction(MainWindow);
        actionStep_Out->setObjectName(QStringLiteral("actionStep_Out"));
        actionStep_Out->setEnabled(false);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.debug.step.out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStep_Out->setIcon(icon11);
        actionStep_Over = new QAction(MainWindow);
        actionStep_Over->setObjectName(QStringLiteral("actionStep_Over"));
        actionStep_Over->setEnabled(false);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.debug.step.over.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStep_Over->setIcon(icon12);
        actionExit_Debug_Mode = new QAction(MainWindow);
        actionExit_Debug_Mode->setObjectName(QStringLiteral("actionExit_Debug_Mode"));
        actionExit_Debug_Mode->setEnabled(false);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icons/Tetra Resources/icons/appbar.bug.remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit_Debug_Mode->setIcon(icon13);
        actionTab_Width = new QAction(MainWindow);
        actionTab_Width->setObjectName(QStringLiteral("actionTab_Width"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QStringLiteral("background: rgb(255, 255, 255)"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setStyleSheet(QLatin1String("QSplitter::handle{\n"
"	background-color: white;\n"
"}"));
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        input = new Editor(layoutWidget);
        input->setObjectName(QStringLiteral("input"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(input->sizePolicy().hasHeightForWidth());
        input->setSizePolicy(sizePolicy);
        input->setSizeIncrement(QSize(1, 1));
        input->setMouseTracking(false);
        input->setContextMenuPolicy(Qt::DefaultContextMenu);
        input->setStyleSheet(QLatin1String("Editor{\n"
"border: 1px solid grey\n"
"}"));

        gridLayout->addWidget(input, 2, 0, 1, 1);

        tetraFileLabel = new QLabel(layoutWidget);
        tetraFileLabel->setObjectName(QStringLiteral("tetraFileLabel"));

        gridLayout->addWidget(tetraFileLabel, 0, 0, 1, 1);

        cursorPosition = new QLabel(layoutWidget);
        cursorPosition->setObjectName(QStringLiteral("cursorPosition"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cursorPosition->sizePolicy().hasHeightForWidth());
        cursorPosition->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(cursorPosition, 0, 1, 1, 1);

        threadMdi = new QMdiArea(layoutWidget);
        threadMdi->setObjectName(QStringLiteral("threadMdi"));
        threadMdi->setStyleSheet(QStringLiteral("background-color{qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.05 rgba(14, 8, 73, 255), stop:0.36 rgba(28, 17, 145, 255), stop:0.6 rgba(126, 14, 81, 255), stop:0.75 rgba(234, 11, 11, 255), stop:0.79 rgba(244, 70, 5, 255), stop:0.86 rgba(255, 136, 0, 255), stop:0.935 rgba(239, 236, 55, 255));}"));

        gridLayout->addWidget(threadMdi, 1, 0, 1, 1);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        outputLabel = new QLabel(layoutWidget1);
        outputLabel->setObjectName(QStringLiteral("outputLabel"));

        verticalLayout->addWidget(outputLabel);

        output = new QTextEdit(layoutWidget1);
        output->setObjectName(QStringLiteral("output"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(output->sizePolicy().hasHeightForWidth());
        output->setSizePolicy(sizePolicy2);
        output->setAutoFillBackground(false);
        output->setStyleSheet(QLatin1String("QTextEdit{\n"
"border: 1px solid grey;\n"
"}"));
        output->setFrameShadow(QFrame::Plain);
        output->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        output->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        output->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        output->setLineWrapMode(QTextEdit::WidgetWidth);
        output->setReadOnly(true);

        verticalLayout->addWidget(output);

        splitter->addWidget(layoutWidget1);

        verticalLayout_2->addWidget(splitter);

        userInput = new QPlainTextEdit(centralWidget);
        userInput->setObjectName(QStringLiteral("userInput"));

        verticalLayout_2->addWidget(userInput);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout_2->addWidget(pushButton);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 868, 29));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuBuild = new QMenu(menuBar);
        menuBuild->setObjectName(QStringLiteral("menuBuild"));
        menuDebug = new QMenu(menuBar);
        menuDebug->setObjectName(QStringLiteral("menuDebug"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QStringLiteral("menuWindow"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setEnabled(true);
        toolBar->setAutoFillBackground(true);
        toolBar->setStyleSheet(QStringLiteral(""));
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(32, 32));
        toolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuBuild->menuAction());
        menuBar->addAction(menuDebug->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuHelp->addAction(actionDocumentation);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout_Tetra);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFind);
        menuBuild->addAction(actionRun);
        menuBuild->addSeparator();
        menuBuild->addAction(actionBuild);
        menuBuild->addAction(actionStop);
        menuDebug->addAction(actionDebug);
        menuDebug->addAction(actionResume);
        menuDebug->addAction(actionPause);
        menuDebug->addAction(actionExit_Debug_Mode);
        menuWindow->addAction(actionMinimize);
        menuWindow->addAction(actionFull_Screen);
        menuWindow->addSeparator();
        menuWindow->addAction(actionClear_Output);
        menuWindow->addSeparator();
        menuWindow->addAction(actionTab_Width);
        menuWindow->addAction(actionLine_Numbers);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionPrint);
        toolBar->addSeparator();
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRedo);
        toolBar->addSeparator();
        toolBar->addAction(actionBuild);
        toolBar->addAction(actionRun);
        toolBar->addAction(actionStop);
        toolBar->addSeparator();
        toolBar->addAction(actionDebug);
        toolBar->addAction(actionExit_Debug_Mode);
        toolBar->addSeparator();
        toolBar->addAction(actionResume);
        toolBar->addAction(actionPause);
        toolBar->addAction(actionStep_Through);
        toolBar->addAction(actionStep_Over);
        toolBar->addAction(actionStep_Out);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionPrint->setText(QApplication::translate("MainWindow", "Print...", 0));
        actionAbout_Tetra->setText(QApplication::translate("MainWindow", "About Tetra", 0));
        actionDocumentation->setText(QApplication::translate("MainWindow", "Documentation", 0));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", 0));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0));
        actionDebug->setText(QApplication::translate("MainWindow", "Debug", 0));
        actionOpen_Recent->setText(QApplication::translate("MainWindow", "Open Recent", 0));
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", 0));
        actionBuild->setText(QApplication::translate("MainWindow", "Build", 0));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        actionFind->setText(QApplication::translate("MainWindow", "Find", 0));
        actionMinimize->setText(QApplication::translate("MainWindow", "Minimize", 0));
        actionFull_Screen->setText(QApplication::translate("MainWindow", "Full Screen", 0));
        actionLine_Numbers->setText(QApplication::translate("MainWindow", "Line Numbers", 0));
        actionClear_Output->setText(QApplication::translate("MainWindow", "Clear Output", 0));
        actionResume->setText(QApplication::translate("MainWindow", "Resume", 0));
        actionPause->setText(QApplication::translate("MainWindow", "Pause", 0));
        actionStep_Through->setText(QApplication::translate("MainWindow", "Step Through", 0));
        actionStep_Out->setText(QApplication::translate("MainWindow", "Step Out", 0));
        actionStep_Over->setText(QApplication::translate("MainWindow", "Step Over", 0));
        actionExit_Debug_Mode->setText(QApplication::translate("MainWindow", "Exit Debug Mode", 0));
        actionTab_Width->setText(QApplication::translate("MainWindow", "Tab Width", 0));
        tetraFileLabel->setText(QApplication::translate("MainWindow", "Tetra File:", 0));
        cursorPosition->setText(QApplication::translate("MainWindow", "0, 0", 0));
        outputLabel->setText(QApplication::translate("MainWindow", "Output:", 0));
        output->setPlaceholderText(QString());
        pushButton->setText(QApplication::translate("MainWindow", "Enter", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuBuild->setTitle(QApplication::translate("MainWindow", "Build", 0));
        menuDebug->setTitle(QApplication::translate("MainWindow", "Debug", 0));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Window", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
