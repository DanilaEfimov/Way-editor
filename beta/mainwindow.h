#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    static std::map<uint, Graph*> graphs;

    static MenuBar* mainMenu;
    static QIcon* icon;

    void connectTabWithMenu();
signals:

};
#endif // MAINWINDOW_H
