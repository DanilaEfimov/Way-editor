#include "mainwindow.h"
#include "ui_mainwindow.h"

MenuBar* MainWindow::mainMenu = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Ways Editor");
    mainMenu = new MenuBar;
    this->setMenuBar(mainMenu->getMenuBar());
    ui->tabWidget->setTabText(0, "Notes");
    ui->tabWidget->setTabText(1, "Console");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mainMenu;
}
