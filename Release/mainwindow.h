#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QCheckBox>
#include <map>
#include "Graphs/Graph.h"
#include "Parser.h"
#include "Error.h"

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
    friend class Error;
    friend class Parser;

    Ui::MainWindow* ui;
    static QIcon* icon;
    static std::map<uint, Graph*> graphs;
    static std::map<uint, QTextEdit*> fields;

    static QCheckBox* ErrorReturned;
    static QCheckBox* WarningReturned;

    static Parser parser;

    // INITIALIZE
    void initMenu();
    void initWidgetsView();
    void initInputArea();
    void initOutputArea();
    void initStatusBar();
    void initWindow();

    void binding();
    void bindFileMenu();
    void bindViewMenu();
    void bindInfoMenu();
    // it's about Console line menu
private slots:
    // MENUES
    // FILE MENU
    void newFile();
    void saveFile() const;
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
