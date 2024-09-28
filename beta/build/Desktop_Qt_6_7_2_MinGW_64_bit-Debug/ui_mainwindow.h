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
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
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
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *generalLayout;
    QTabWidget *fields;
    QWidget *Notes;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *notes;
    QWidget *Console;
    QVBoxLayout *verticalLayout;
    QTextEdit *cmds;
    QTextEdit *infoGraph;
    QMenuBar *MENU;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_4 = new QHBoxLayout(centralwidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        generalLayout = new QHBoxLayout();
        generalLayout->setObjectName("generalLayout");
        fields = new QTabWidget(centralwidget);
        fields->setObjectName("fields");
        Notes = new QWidget();
        Notes->setObjectName("Notes");
        horizontalLayout_2 = new QHBoxLayout(Notes);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        notes = new QTextEdit(Notes);
        notes->setObjectName("notes");

        horizontalLayout_2->addWidget(notes);

        fields->addTab(Notes, QString());
        Console = new QWidget();
        Console->setObjectName("Console");
        verticalLayout = new QVBoxLayout(Console);
        verticalLayout->setObjectName("verticalLayout");
        cmds = new QTextEdit(Console);
        cmds->setObjectName("cmds");

        verticalLayout->addWidget(cmds);

        fields->addTab(Console, QString());

        generalLayout->addWidget(fields);

        infoGraph = new QTextEdit(centralwidget);
        infoGraph->setObjectName("infoGraph");

        generalLayout->addWidget(infoGraph);


        horizontalLayout_4->addLayout(generalLayout);

        MainWindow->setCentralWidget(centralwidget);
        MENU = new QMenuBar(MainWindow);
        MENU->setObjectName("MENU");
        MENU->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(MENU);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        fields->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        fields->setTabText(fields->indexOf(Notes), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        fields->setTabText(fields->indexOf(Console), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
