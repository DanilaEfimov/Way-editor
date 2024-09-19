#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "General.h"

MenuBar* MainWindow::mainMenu = nullptr;
QIcon* MainWindow::icon = nullptr;
const QPalette* MainWindow::black = new QPalette(BLACK_BUTTON_THEME, BLACK_WINDOW_THEME);
const QPalette* MainWindow::white = new QPalette(WHITE_BUTTON_THEME, WHITE_WINDOW_THEME);
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
    this->setBlackTheme();
    ui->tabWidget->setTabText(0, "Notes");
    ui->tabWidget->removeTab(1);            // default User have only note tab

    ui->infoGraph->setReadOnly(true);

    this->connectViewMenu();
}

MainWindow::~MainWindow() {
    delete ui;
    delete icon;
    delete mainMenu;
}

void MainWindow::connectViewMenu() {
    connect(mainMenu->getAction(Names::blackTheme), SIGNAL(triggered(bool)), this, SLOT(setBlackTheme()));
    connect(mainMenu->getAction(Names::whiteTheme), SIGNAL(triggered(bool)), this, SLOT(setWhiteTheme()));
    connect(mainMenu->getAction(Names::rightMode),  SIGNAL(triggered(bool)), this, SLOT(setRightDirection()));
    connect(mainMenu->getAction(Names::leftMode),   SIGNAL(triggered(bool)), this, SLOT(setLeftDirection()));
}

void MainWindow::connectLayoutWithMenu()
{

}

void MainWindow::connectWindowWithMenu()
{

}

void MainWindow::setBlackTheme(){
    this->setPalette(*black);
    mainMenu->setPalette(*black);
}

void MainWindow::setWhiteTheme(){
    this->setPalette(*white);
    mainMenu->setPalette(*white);
}

void MainWindow::setRightDirection() {
    ui->centralwidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
}

void MainWindow::setLeftDirection() {
    ui->centralwidget->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
}
