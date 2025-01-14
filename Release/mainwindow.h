#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QCheckBox>
#include <QKeyEvent>
#include <map>
#include "Graphs/Graph.h"
#include "PerformanceManager.h"

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
    friend class PerformanceManager;

    static Ui::MainWindow* ui;
    static QIcon* icon;
    static std::map<uint, Graph*> graphs;
    static std::map<uint, QTextEdit*> fields;

    static QCheckBox* ErrorReturned;
    static QCheckBox* WarningReturned;

    // KEY EVENTS
    void backSpaceEvent(uint curTab, QString& text);    // in .cpp file it contains near keyPressEvent
    void enterCmdEvent(uint curTab, QString& cmd, std::string& argv, /* for performance */
                    QString& qText, QString& graphConectList,   /* for updating output */
                    std::string& stdcmd, std::string& lastText, std::string& text);
    void defaultKeyEvent(uint curTab, QKeyEvent* e, std::string& lastText, std::string& newText, std::string& text);

    void updateHystoru(std::string& cmd);
    void clearHystory();
    void clearInput();

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
    void bindLineMenu();

private slots:
    /* MENUES */
    // FILE MENU
    void newFile();
    void saveFile();
    // VIEW MENU
    void setBlackTheme();
    void setWhiteTheme();
    void setRightMode();
    void setLeftMode();
    // COMMAND LINE MENU
    void help();
    void hystory();

    // input settings
    virtual void keyPressEvent(QKeyEvent* e) override;
};

#endif // MAINWINDOW_H
