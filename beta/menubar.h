#ifndef MENUBAR_H
#define MENUBAR_H

#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QMainWindow>

enum {
    newSession, save, whiteTheme, blackTheme,
    rightMode, leftMode, help, openLine, history
};

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    MenuBar();
    ~MenuBar();

    // Accessors
    static QMenuBar* getMenuBar();
    static QMenu* getMenu(uint index);
    static QAction* getAction(uint index);

private:
    static QMenuBar* bar;

    static const uint menuc = 3;    // file, view, command line
    static const uint actc = 9;
    // file: save, new session;
    // view: white theme, black theme, right mode, left mode;
    // command line: help, open line, history;

    static QMenu** menues;
    static QAction** actions;

    // Construct
    static void initActions();
    static void initMenues();
    static void bindActions();
    static void bindMenues();

    // Destruct
    static void deleteActions();
    static void deleteMenues();

};

#endif // MENUBAR_H
