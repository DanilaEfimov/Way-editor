/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *New_file;
    QAction *Save;
    QAction *Black_theme;
    QAction *White_theme;
    QAction *Right_mode;
    QAction *Left_mode;
    QAction *Help;
    QAction *Hystory;
    QWidget *mainLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *Console;
    QTabWidget *inputArea;
    QWidget *workArea;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *notes;
    QWidget *nothing;
    QTextEdit *outputArea;
    QMenuBar *mainMenu;
    QMenu *File;
    QMenu *View;
    QMenu *Command_line;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(428, 345);
        MainWindow->setMinimumSize(QSize(428, 345));
        New_file = new QAction(MainWindow);
        New_file->setObjectName("New_file");
        Save = new QAction(MainWindow);
        Save->setObjectName("Save");
        Black_theme = new QAction(MainWindow);
        Black_theme->setObjectName("Black_theme");
        White_theme = new QAction(MainWindow);
        White_theme->setObjectName("White_theme");
        Right_mode = new QAction(MainWindow);
        Right_mode->setObjectName("Right_mode");
        Left_mode = new QAction(MainWindow);
        Left_mode->setObjectName("Left_mode");
        Help = new QAction(MainWindow);
        Help->setObjectName("Help");
        Hystory = new QAction(MainWindow);
        Hystory->setObjectName("Hystory");
        mainLayout = new QWidget(MainWindow);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setMinimumSize(QSize(428, 306));
        verticalLayout = new QVBoxLayout(mainLayout);
        verticalLayout->setObjectName("verticalLayout");
        Console = new QHBoxLayout();
        Console->setObjectName("Console");
        inputArea = new QTabWidget(mainLayout);
        inputArea->setObjectName("inputArea");
        workArea = new QWidget();
        workArea->setObjectName("workArea");
        horizontalLayout_2 = new QHBoxLayout(workArea);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        notes = new QTextEdit(workArea);
        notes->setObjectName("notes");

        horizontalLayout_2->addWidget(notes);

        inputArea->addTab(workArea, QString());
        nothing = new QWidget();
        nothing->setObjectName("nothing");
        inputArea->addTab(nothing, QString());

        Console->addWidget(inputArea);

        outputArea = new QTextEdit(mainLayout);
        outputArea->setObjectName("outputArea");

        Console->addWidget(outputArea);


        verticalLayout->addLayout(Console);

        MainWindow->setCentralWidget(mainLayout);
        mainMenu = new QMenuBar(MainWindow);
        mainMenu->setObjectName("mainMenu");
        mainMenu->setGeometry(QRect(0, 0, 428, 17));
        File = new QMenu(mainMenu);
        File->setObjectName("File");
        View = new QMenu(mainMenu);
        View->setObjectName("View");
        Command_line = new QMenu(mainMenu);
        Command_line->setObjectName("Command_line");
        MainWindow->setMenuBar(mainMenu);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        mainMenu->addAction(File->menuAction());
        mainMenu->addAction(View->menuAction());
        mainMenu->addAction(Command_line->menuAction());
        File->addAction(New_file);
        File->addAction(Save);
        View->addAction(Black_theme);
        View->addAction(White_theme);
        View->addAction(Right_mode);
        View->addAction(Left_mode);
        Command_line->addAction(Help);
        Command_line->addAction(Hystory);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        New_file->setText(QCoreApplication::translate("MainWindow", "New file", nullptr));
        Save->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        Black_theme->setText(QCoreApplication::translate("MainWindow", "Black theme", nullptr));
        White_theme->setText(QCoreApplication::translate("MainWindow", "White theme", nullptr));
        Right_mode->setText(QCoreApplication::translate("MainWindow", "Right mode", nullptr));
        Left_mode->setText(QCoreApplication::translate("MainWindow", "Left mode", nullptr));
        Help->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));
        Hystory->setText(QCoreApplication::translate("MainWindow", "Hystory", nullptr));
        inputArea->setTabText(inputArea->indexOf(workArea), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        inputArea->setTabText(inputArea->indexOf(nothing), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        File->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        View->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        Command_line->setTitle(QCoreApplication::translate("MainWindow", "Command line", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
