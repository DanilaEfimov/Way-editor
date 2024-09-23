#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "General.h"

#include<QFileDialog>
#include<QDateTime>
// STL
#include<fstream>

MenuBar* MainWindow::mainMenu = nullptr;
QIcon* MainWindow::icon = nullptr;
const QPalette* MainWindow::black = new QPalette(BLACK_BUTTON_THEME, BLACK_WINDOW_THEME);
const QPalette* MainWindow::white = new QPalette(WHITE_BUTTON_THEME, WHITE_WINDOW_THEME);
std::map<uint, Graph*> MainWindow::graphs = {};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // basic init
    ui->setupUi(this);
    this->setWindowTitle(APP_NAME);
    icon = new QIcon(ICON_PATH);
    this->setWindowIcon(*icon);

    // menubar
    mainMenu = new MenuBar;
    this->setMenuBar(mainMenu->getMenuBar());
    this->setBlackTheme();
    ui->fields->setTabText(0, "Notes");
    ui->fields->removeTab(1);            // default User have only note tab

    // text edit
    ui->infoGraph->setReadOnly(true);

    // binding signals with fonts
    this->connectWindowWithMenu();
}

MainWindow::~MainWindow() { // fix it soon
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

void MainWindow::connectFileMenu() {
    connect(mainMenu->getAction(Names::newSession), SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(mainMenu->getAction(Names::save),       SIGNAL(triggered(bool)), this, SLOT(saveFile()));
}

void MainWindow::connectCommandMenu()
{

}

void MainWindow::connectWindowWithMenu()
{
    this->connectFileMenu();
    this->connectViewMenu();
}

void MainWindow::setBlackTheme(){
    this->setPalette(*black);
    mainMenu->setPalette(*black);
    ui->fields->setPalette(*black);
    ui->infoGraph->setPalette(*black);
}

void MainWindow::setWhiteTheme(){
    this->setPalette(*white);
    mainMenu->setPalette(*white);
    ui->fields->setPalette(*white);
    ui->infoGraph->setPalette(*white);
}

void MainWindow::setRightDirection() {
    ui->centralwidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
}

void MainWindow::setLeftDirection() {
    ui->centralwidget->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
}

void MainWindow::openFile() {
    QString path = "";
    path = QFileDialog::getOpenFileName(this, QObject::tr("Choose graph file"), QDir::homePath(), 0);
    if(path.length()){
        Graph* newG = new Graph(path.toStdString());
        graphs.emplace(std::pair<int, Graph*>(newG->getID(), newG));

        ui->infoGraph->setText(newG->show());
        ui->fields->addTab(new QTextEdit, path);
        ui->fields->setCurrentIndex(ui->fields->count() - 1);
    }
}

void MainWindow::saveFile() {
    int currentTab = ui->fields->currentIndex(); // one less than current graph id
    std::string path = ui->fields->tabText(currentTab).toStdString();
    std::fstream savedFile(path);
    if(!savedFile.is_open()){
        QMessageBox fail;
        fail.setInformativeText(FAILED_TO_OPEN);
        fail.setWindowTitle("Fail");
        fail.exec();
        return;
    }

    QString res = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy") + '\n'; // windows system time API
    savedFile << res.toStdString();
    savedFile << graphs[currentTab - 1]->show().toStdString();
    savedFile.close();
}
