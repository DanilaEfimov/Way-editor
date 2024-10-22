#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "General.h"

#include<QFileDialog>
#include<QDateTime>
#include<QDir>
#include<QKeyEvent>
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
    ui->fields->setMovable(false);

    // text edit
    ui->infoGraph->setReadOnly(true);

    // binding signals with slots
    this->connectWindowWithMenu();
    this->connectWindowWithConsole();
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

void MainWindow::connectCommandMenu() {
    connect(mainMenu->getAction(Names::help),       SIGNAL(triggered(bool)), this, SLOT(helpInfo()));
    connect(mainMenu->getAction(Names::history),    SIGNAL(triggered(bool)), this, SLOT(showHystory()));
}

void MainWindow::connectWindowWithMenu() {
    this->connectFileMenu();
    this->connectViewMenu();
    this->connectCommandMenu();
}

void MainWindow::connectWindowWithConsole() {
    //connect(this, SIGNAL(QMainWindow::enterEvent()), this, SLOT(keyPressEvent(QKeyEvent*)));
}

// SIGNALS

void MainWindow::keyPressEvent(QKeyEvent* event){
    int key = event->key();

    // current tab shouldn't be 'Note' = 0
    // and current tab have to be focused by text cursor
    if(ui->fields->hasFocus() &&
        ui->fields->currentIndex())
    {
        switch(key){
        case Qt::Key_Enter:
            parsing();
            break;
        default:
            break;
        }
    }
}

// SLOTS

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

        QTextEdit* newConsole = new QTextEdit(this);
        newConsole->setOverwriteMode(true);
        newConsole->setText(START_SESSION);
        ui->infoGraph->setText(newG->show());
        ui->fields->addTab(newConsole, path);
        ui->fields->setCurrentIndex(ui->fields->count() - 1); // nums from 0, but count from 1
    }

    // would user choose random file (not .vl/.el/.mat)
    // will be inited empty graph
}

void MainWindow::saveFile() { // not fixed yet
    int currentTab = ui->fields->currentIndex(); // one less than current graph id
    std::string path = ui->fields->tabText(currentTab).toStdString();
    std::fstream savedFile;
    if(getExtension(path) != undefined){
            savedFile.open(path, std::ios::app);
    }
    if(!savedFile.is_open()){
        // in alpha version will be maded message box function
        // because here are many same msgBox calls
        // only text difference
        QMessageBox fail;
        fail.setInformativeText(FAILED_TO_OPEN);
        fail.setWindowTitle("Fail");
        fail.setIcon(QMessageBox::Information);
        fail.exec();
        return;
    }

    graphs[currentTab]->show(ui->fields->tabText(currentTab).toStdString());
}

void MainWindow::helpInfo() {

    std::string cur = QDir::currentPath().toStdString();
    std::fstream help(cur + HELP_NAME); // it's such cringe...
    if(!help.is_open()){
        QMessageBox fail;
        fail.setInformativeText(FAILED_TO_OPEN);
        fail.setWindowTitle("Fail");
        fail.exec();
        return;
    }

    QString res = "";
    while(!help.eof()){
        std::string tempS;
        std::getline(help, tempS);
        res += tempS + '\n';
    }

    ui->infoGraph->setText(res);
}

void MainWindow::showHystory() {
    std::string cur = QDir::currentPath().toStdString();
    std::fstream history(cur + HISTORY_NAME); // it's such cringe...
    if(!history.is_open()){
        QMessageBox fail;
        fail.setInformativeText(FAILED_TO_OPEN);
        fail.setWindowTitle("Fail");
        fail.exec();
        return;
    }

    QString res = "";
    while(!history.eof()){
        std::string tempS;
        std::getline(history, tempS);
        res += tempS + '\n';
    }

    ui->infoGraph->setText(res);
}

void MainWindow::parsing() {
    QTextEdit* currTab = (QTextEdit*)ui->fields->currentWidget();
    QString allText = currTab->toPlainText();
    std::string lastComand = "";

    std::stringstream allTextStream(allText.toStdString());
    while(std::getline(allTextStream, lastComand));

    qInfo() << lastComand;
}
