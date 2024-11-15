#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <map>
#include "Graphs/Graph.h"

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
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    static QIcon* icon;
    static std::map<uint, Graph*> graphs;
    static std::map<uint, QTextEdit*> fields;

    // INITIALIZE
    void initMenu();
    void initInputArea();
    void initOutputArea();
    void initWindow();
    void binding();

    friend void bindFileMenu(QMenuBar* mainMenu, MainWindow& mainWindow);
    friend void bindViewMenu(QMenuBar* mainMenu, MainWindow& mainWindow);
    friend void bindInfoMenu(QMenuBar* mainMenu, MainWindow& mainWindow); // it's about Console line menu
private slots:
    // MENUES
    // FILE MENU
    void newFile(const char* path);
    void saveFile(const char* path) const;
    // VIEW MENU
    void setBlackTheme();
    void setWhiteTheme();
    void setRightMode();
    void setLeftMode();
    // COMMAND LINE MENU
    void help();
    void hystory();
};

#endif // MAINWINDOW_H
