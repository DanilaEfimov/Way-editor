#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include "General.h"

QRadioButton* MainWindow::ErrorReturned = nullptr;
QRadioButton* MainWindow::WarningReturned = nullptr;
std::map<uint, Graph*> MainWindow::graphs = std::map<uint, Graph*>{};
std::map<uint, QTextEdit*> MainWindow::fields = std::map<uint, QTextEdit*>{};
QIcon* MainWindow::icon = nullptr;

// BINDIG SPECIAL FOR EVERY MENU

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->initWindow();
    this->initMenu();
    this->initInputArea();
    this->initOutputArea();
    this->initWidgetsView();
}

MainWindow::~MainWindow() {
    delete ui;
    delete ErrorReturned;
    delete WarningReturned;
    for(auto& graph : this->graphs){
        delete graph.second;
    }
    for(auto& area : this->fields){
        delete area.second;
    }
}

void MainWindow::initMenu() {
    this->binding();
}

void MainWindow::initWidgetsView() {
    // Tab Widget "input Area"
    ui->inputArea->setTabText(0, "Hello, Way Editor!");
    ui->inputArea->removeTab(1);
    ui->notes->insertPlainText(_HELLO_);
    this->setBlackTheme();
    this->setLeftMode();
}

void MainWindow::initInputArea() {
    ui->notes->setReadOnly(true);
}

void MainWindow::initOutputArea() {
    ui->outputArea->setReadOnly(true);
}

void MainWindow::initStatusBar() {
    ErrorReturned =  new QRadioButton(ui->returns);
    WarningReturned = new QRadioButton(ui->returns);
    ErrorReturned->setText(_ERROR_);
    ErrorReturned->setChecked(false);
    ErrorReturned->setCheckable(false);
    const QPalette errorTheme(ERROR_BUTTON_THEME);
    ErrorReturned->setPalette(errorTheme);
    WarningReturned->setText(_WARNING_);
    WarningReturned->setChecked(false);
    WarningReturned->setCheckable(false);
    const QPalette warningTheme(WARNING_BUTTON_THEME);
    WarningReturned->setPalette(warningTheme);
    ui->returns->addWidget(ErrorReturned);
    ui->returns->addWidget(WarningReturned);
}

void MainWindow::initWindow() {
    this->icon = new QIcon(ICON_PATH);
    this->setWindowIcon(*icon);
    this->setWindowTitle(APP_NAME);
    this->initStatusBar();
}

void MainWindow::binding() {
    this->bindFileMenu();
    this->bindViewMenu();
    this->bindInfoMenu();
}

void MainWindow::bindFileMenu() {
    connect(ui->New_file,   SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(ui->Save,       SIGNAL(triggered(bool)), this, SLOT(saveFile()));
}

void MainWindow::bindViewMenu() {
    connect(ui->Black_theme,SIGNAL(triggered(bool)), this, SLOT(setBlackTheme()));
    connect(ui->White_theme,SIGNAL(triggered(bool)), this, SLOT(setWhiteTheme()));
    connect(ui->Right_mode, SIGNAL(triggered(bool)), this, SLOT(setRightMode()));
    connect(ui->Left_mode,  SIGNAL(triggered(bool)), this, SLOT(setLeftMode()));
}

void MainWindow::bindInfoMenu()
{

}

void MainWindow::newFile() {

}

void MainWindow::saveFile() const {

}

void MainWindow::setBlackTheme() {
    const QPalette blackTheme(BLACK_BUTTON_THEME, BLACK_WINDOW_THEME);
    this->setPalette(blackTheme);
    ui->mainMenu->setPalette(blackTheme);
    ui->inputArea->setPalette(blackTheme);
    ui->outputArea->setPalette(blackTheme);
    ui->returns->setPalette(blackTheme);
}

void MainWindow::setWhiteTheme() {
    const QPalette whiteTheme(WHITE_BUTTON_THEME, WHITE_WINDOW_THEME);
    this->setPalette(whiteTheme);
    ui->mainMenu->setPalette(whiteTheme);
    ui->inputArea->setPalette(whiteTheme);
    ui->outputArea->setPalette(whiteTheme);
    ui->returns->setPalette(whiteTheme);
}

void MainWindow::setRightMode() {
    ui->central->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
}

void MainWindow::setLeftMode() {
    ui->central->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
}


void MainWindow::help() {

}

void MainWindow::hystory() {

}
