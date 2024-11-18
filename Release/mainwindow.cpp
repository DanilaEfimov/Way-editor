#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include "General.h"

std::map<uint, Graph*> MainWindow::graphs = std::map<uint, Graph*>{};
std::map<uint, QTextEdit*> MainWindow::fields = std::map<uint, QTextEdit*>{};
QIcon* MainWindow::icon = nullptr;

// BINDIG SPECIAL FOR EVERY MENU

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // ICON
    this->icon = new QIcon(ICON_PATH);
    this->setWindowIcon(*icon);
    this->initWindow();
    this->initInputArea();
    this->initOutputArea();
    this->initWidgetsView();
    this->binding();
}

MainWindow::~MainWindow() {
    delete ui;
    for(auto& graph : this->graphs){
        delete graph.second;
    }
    for(auto& area : this->fields){
        delete area.second;
    }
}

void MainWindow::initMenu() {

}

void MainWindow::initWidgetsView() {
    // Tab Widget "input Area"
    ui->inputArea->setTabText(0, "Hello, Way Editor!");
    ui->inputArea->removeTab(1);
    ui->notes->insertPlainText(_HELLO_);
}

void MainWindow::initInputArea() {
    ui->notes->setReadOnly(true);
}

void MainWindow::initOutputArea() {
    ui->outputArea->setReadOnly(true);
}

void MainWindow::initWindow() {
    this->setWindowTitle(APP_NAME);
    this->setBlackTheme();
    this->setLeftMode();
}

void MainWindow::binding() {

}

void MainWindow::newFile(const char *path) {

}

void MainWindow::saveFile(const char *path) const {

}

void MainWindow::setBlackTheme() {
    const QPalette blackTheme(BLACK_BUTTON_THEME, BLACK_WINDOW_THEME);
    this->setPalette(blackTheme);
    ui->mainMenu->setPalette(blackTheme);
    ui->inputArea->setPalette(blackTheme);
    ui->outputArea->setPalette(blackTheme);
}

void MainWindow::setWhiteTheme() {

}

void MainWindow::setRightMode() {

}

void MainWindow::setLeftMode() {

}

void MainWindow::help() {

}

void MainWindow::hystory() {

}
