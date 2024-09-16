#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "General.h"

MenuBar* MainWindow::mainMenu = nullptr;
QIcon* MainWindow::icon = nullptr;
std::map<uint, Graph*> MainWindow::graphs = {};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(APP_NAME);
    icon = new QIcon(ICON_PATH);
    this->setWindowIcon(*icon);

    mainMenu = new MenuBar;
    this->setMenuBar(mainMenu->getMenuBar());
    ui->tabWidget->setTabText(0, "Notes");
    ui->tabWidget->removeTab(1);            // default User have only note tab

    this->connectTabWithMenu();
}

MainWindow::~MainWindow() {
    delete ui;
    delete icon;
    delete mainMenu;
}

void MainWindow::connectTabWithMenu() {
    connect(mainMenu->getAction(newSession), SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(save),       SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(whiteTheme), SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(blackTheme), SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(rightMode),  SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(leftMode),   SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(help),       SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(openLine),   SIGNAL(), ui->tabWidget, SLOT());
    connect(mainMenu->getAction(history),    SIGNAL(), ui->tabWidget, SLOT());
}

