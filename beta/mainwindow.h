#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPalette>
#include <QIcon>
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

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;

    static const QPalette* white;
    static const QPalette* black;

    static std::map<uint, Graph*> graphs;

    static MenuBar* mainMenu;
    static QIcon* icon;

    // Binding
    void connectViewMenu();
    void connectFileMenu();
    void connectLayoutWithMenu();
    void connectWindowWithMenu();
private slots:
    // Menues
    void setBlackTheme();
    void setWhiteTheme();
    void setRightDirection();
    void setLeftDirection();
    void openFile();
    void saveFile();
};
#endif // MAINWINDOW_H
