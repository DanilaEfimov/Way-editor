#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "General.h"

#include<QFileDialog>
#include<QDir>
#include<QKeyEvent>
// STL
#include<fstream>
#include<sstream>
#include<string>

MenuBar* MainWindow::mainMenu = nullptr;
QIcon* MainWindow::icon = nullptr;
const QPalette* MainWindow::black = new QPalette(BLACK_BUTTON_THEME, BLACK_WINDOW_THEME);
const QPalette* MainWindow::white = new QPalette(WHITE_BUTTON_THEME, WHITE_WINDOW_THEME);
std::map<uint, Graph*> MainWindow::graphs = {};
std::map<uint, QTextEdit*> MainWindow::pages = {};
bool MainWindow::errorCall = false;

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
    this->connectOutput();
}

MainWindow::~MainWindow() { // fix it soon
    delete ui;
    delete icon;
    delete mainMenu;
    for(auto& g : graphs){
        delete g.second;
    }
    qApp->exit(0);
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

void MainWindow::connectWindowWithConsole(uint index) {
    if(index){
        connect(this->pages[index], SIGNAL(textChanged()), this, SLOT(keyPressed()));
    }
}

void MainWindow::connectOutput() {
    connect(this, SIGNAL(executed(std::string&)), this, SLOT(updateTexts(std::string&)));
}

void MainWindow::updateHistory(std::string &cmd) {
    QDateTime time = QDateTime::currentDateTime();
    std::string timeDate = time.toString().toStdString();
    std::fstream history(HISTORY_NAME, std::ios_base::app);
    if(!history.is_open()){
        errorMassege(FAILED_TO_OPEN);
    }
    history << cmd << " " << timeDate << std::endl;
    history.close();
}

// SLOTS

void MainWindow::keyPressed(){
    // awfull thing... because I don't understand well signals & slots
    // current tab shouldn't be 'Note' = 0
    // and current tab have to be focused by text cursor
    if(!errorCall){
        int current = ui->fields->currentIndex();
        std::string lastCmd = "";
        std::string allText = pages[current]->toPlainText().toStdString();

        std::stringstream text(allText);
        while(!text.eof()){ // here we find last cmd's text
            std::getline(text, lastCmd);    // O(lines)
        }

        if(lastCmd == CMD_FLAG && !errorCall){
            allText.erase(--allText.end()); // last char is '\0' - CMD_FLAG
            std::stringstream CMDtext(allText);
            while(!CMDtext.eof()){
                std::getline(CMDtext, lastCmd);
            }
            std::string cmd = graphs[current]->execution(lastCmd);
            emit this->executed(cmd);
        }
    }
    else{
        errorCall = false;
    }
    // if last command text is "" ("\0") it means that
    // it was inputed yet and we have to pars this
}

void MainWindow::updateTexts(std::string &answer) {
    uint current = ui->fields->currentIndex();
    QString newText = this->graphs[current]->show();
    ui->infoGraph->setText(newText);
    if(answer.find("ERROR") != std::string::npos ||
        answer.find("WARNING") != std::string::npos ||
        answer.find("ANSWER") != std::string::npos){
        errorCall = true;
        // here we check what was last string
    }

    this->updateHistory(answer);
    QString text = "";
    text += answer;
    this->pages[current]->insertPlainText(text);
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
        newConsole->moveCursor(QTextCursor::End);
        ui->infoGraph->setText(newG->show());
        ui->fields->addTab(newConsole, path);

        uint index = ui->fields->count() - 1;   // index from 0, but count from 1
        ui->fields->setCurrentIndex(index);
        pages.emplace(std::make_pair(index, newConsole));
        this->connectWindowWithConsole(index);
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
        errorMassege(FAILED_TO_OPEN);
        return;
    }

    graphs[currentTab]->show(ui->fields->tabText(currentTab).toStdString());
    savedFile.close();
}

void MainWindow::helpInfo() {
    std::string cur = QDir::currentPath().toStdString();
    std::fstream help(cur + HELP_NAME); // it's such cringe...
    if(!help.is_open()){
        errorMassege(FAILED_TO_OPEN);
        return;
    }

    QString res = "";
    while(!help.eof()){
        std::string tempS;
        std::getline(help, tempS);
        res += tempS + '\n';
    }

    ui->infoGraph->setText(res);
    help.close();
}

void MainWindow::showHystory() {
    std::string cur = QDir::currentPath().toStdString();
    std::fstream history(cur + HISTORY_NAME); // it's such cringe...
    if(!history.is_open()){
        errorMassege(FAILED_TO_OPEN);
        return;
    }

    QString res = "";
    while(!history.eof()){
        std::string tempS;
        std::getline(history, tempS);
        res += tempS + '\n';
    }

    ui->infoGraph->setText(res);
    history.close();
}
