#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include"menubar.h"

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

    static MenuBar* mainMenu;

    static void initIcon();
};
#endif // MAINWINDOW_H
