#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>
#include<QDateTime>
#include <QIcon>
#include <QTextEdit>
#include <map>

#include "menubar.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

    static const QPalette* white;
    static const QPalette* black;

    static std::map<uint, Graph*> graphs;
    static std::map<uint, QTextEdit*> pages;

    static MenuBar* mainMenu;
    static QIcon* icon;

    // Binding
    void connectViewMenu();
    void connectFileMenu();
    void connectCommandMenu();
    void connectWindowWithMenu();
    void connectWindowWithConsole(uint index);
    void connectOutput();

    void updateHistory(std::string& cmd);
    // after will be written custom classes
    // insted ui forms
    // now we have strange signals
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
signals:
    void executed(std::string& cmd);
private slots:
    // Menues
    void setBlackTheme();
    void setWhiteTheme();
    void setRightDirection();
    void setLeftDirection();
    void openFile();
    void saveFile();
    void helpInfo();
    void showHystory();

    // Console
    void keyPressed();
    void updateTexts(std::string& cmd);
};
#endif // MAINWINDOW_H
