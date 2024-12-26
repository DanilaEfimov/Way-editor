#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include <Qstring>
#include<QFileDialog>
#include<QDir>
#include <fstream>
#include "General.h"

Parser MainWindow::parser = Parser();
QCheckBox* MainWindow::ErrorReturned = nullptr;
QCheckBox* MainWindow::WarningReturned = nullptr;
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
    ErrorReturned =  new QCheckBox(ui->returns);
    WarningReturned = new QCheckBox(ui->returns);
    ErrorReturned->setText(_ERROR_);
    ErrorReturned->setChecked(false);
    ErrorReturned->setCheckable(true);
    const QPalette errorTheme(ERROR_BUTTON_THEME);
    ErrorReturned->setPalette(errorTheme);
    WarningReturned->setText(_WARNING_);
    WarningReturned->setChecked(false);
    WarningReturned->setCheckable(true);
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

void MainWindow::bindInfoMenu() {

}

void MainWindow::newFile() {    // not Fixed
    QString path = QFileDialog::getOpenFileName(this, QObject::tr("Choose graph file"), QDir::homePath(), NULL);
    bool choosed = path.length() > 0;
    std::string stdpath = path.toStdString();
    if(choosed){
        QTextEdit* newField = new QTextEdit(this);
        newField->setText(_CONSOLE_START_);
        int fileType = parser.getExtention(stdpath);
        int type = parser.getType(stdpath);
        if(fileType == -1 || type == -1){
            Error(_ERROR_FILE_TYPE_);
            return;
        }
        ushort V = parser.getVertexCount(fileType, stdpath);
        byte** mat = parser.initMatrix(fileType, stdpath);
        Graph* graph = parser.initGraph(type, V, mat);
        if(graph->getV() == 0){
            Error(__EMPTY_GRAPH_SETTED__, true);
            newField->setText(_NEW_EMPTY_GRAPH_);
        }
        uint index = this->fields.size() + 1;
        // input settings
        std::pair<uint, Graph*> graphItem(index, graph);
        this->graphs.emplace(graphItem);
        std::pair<uint, QTextEdit*> item(index, newField);
        ui->inputArea->addTab(newField, path);
        // output area settings
        this->fields.emplace(item);
        QString graphConectList = QString::fromStdString(graph->show());
        ui->outputArea->setText(graphConectList);
        return;
    }
    Error(_FILE_NOT_CHOOSED_);
}

void MainWindow::saveFile() const {
    uint id = ui->inputArea->currentIndex();
    //std::string path =
    Graph* temp = graphs[id];
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
