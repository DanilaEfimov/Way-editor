#include "mainwindow.h"
#include "Graphs/UDirGraph.h"
#include "ui_mainwindow.h"
#include "General.h"
#include "Parser.h"
#include "Error.h"
#include <QPalette>
#include <QFileDialog>
#include <QDir>

Ui::MainWindow* MainWindow::ui = new Ui::MainWindow;
QCheckBox* MainWindow::ErrorReturned = nullptr;
QCheckBox* MainWindow::WarningReturned = nullptr;
std::map<uint, Graph*> MainWindow::graphs = std::map<uint, Graph*>{};
std::map<uint, QTextEdit*> MainWindow::fields = std::map<uint, QTextEdit*>{};
QIcon* MainWindow::icon = nullptr;

// BINDIG SPECIAL FOR EVERY MENU

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
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
    ui->notes->setFontPointSize(FONT_SIZE);
}

void MainWindow::initOutputArea() {
    ui->outputArea->setReadOnly(true);
    ui->outputArea->setFontPointSize(FONT_SIZE);
}

void MainWindow::initStatusBar() {
    ErrorReturned =  new QCheckBox(ui->returns);
    WarningReturned = new QCheckBox(ui->returns);
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

void MainWindow::bindInfoMenu() {

}

void MainWindow::newFile() {
    QString path = QFileDialog::getOpenFileName(this, QObject::tr(_CHOOSE_), QDir::homePath(), NULL);
    bool choosed = path.length() > 0;
    std::string stdpath = path.toStdString();
    if(choosed){
        QTextEdit* newField = new QTextEdit(this);
        newField->setFontPointSize(FONT_SIZE);
        newField->setReadOnly(true);
        newField->setText(_CONSOLE_START_);
        int fileType = Parser::getExtention(stdpath);
        int type = Parser::getType(stdpath);
        if(fileType == -1 || type == -1){
            Error(_ERROR_FILE_TYPE_);
            return;
        }
        ushort V = Parser::getVertexCount(fileType, stdpath);
        byte** mat = Parser::initMatrix(fileType, stdpath);
        Graph* graph = Parser::initGraph(type, V, mat);
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
        // view update
        index = this->fields.size();
        ui->inputArea->setCurrentIndex(index);
        return;
    }
    Error(_FILE_NOT_CHOOSED_);
}

void MainWindow::saveFile() const {
    uint id = ui->inputArea->currentIndex();
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

void MainWindow::keyPressEvent(QKeyEvent* e) {
    qDebug() << e->key() << '\n';
    if(!e){return;};
    QMainWindow::keyPressEvent(e);
    uint currentTab = ui->inputArea->currentIndex();
    if(currentTab == 0){return;}
    int argc = -1;
    std::string argv;
    int functionType = -1;
    switch(e->key()){
    case Qt::Key_Backspace:
        if(Parser::lastLineIsEmpty(currentTab)){
            Error(__EMPTY_INPUT__, true);
            return;
        }
        else{
            QString text = fields[currentTab]->toPlainText();
            QString::Iterator it = text.end() - 1;
            text.erase(it);
            fields[currentTab]->setText(text);
            return;
        }
        break;
    case Qt::Key_Return:    // you can convert rather graph pointer types
        {
            int res = 0;
            QString cmd = Parser::getLastLine(currentTab);
            functionType = Parser::commandCode(cmd.toStdString());
            argc = Parser::argc(functionType);
            argv = Parser::argv(cmd.toStdString());
            res = PerformanceManager::operation(functionType, argc, argv, graphs[currentTab]);
            if(res != 0){ Error(_UNDEFINED_ERROR_); return;}
            QString graphConectList = QString::fromStdString(graphs[currentTab]->show());
            ui->outputArea->setText(graphConectList);
            std::string lastText = fields[currentTab]->toPlainText().toStdString();
            std::string newText = e->text().toStdString();
            std::string text = lastText + '\n';
            fields[currentTab]->setText(QString::fromStdString(text));
        }
    break;
    //case Qt::CTRL:  // ctrl + S = save, ctrl + E = exit, ctrl + LShift = decrement tab index ...
    //   break;
    default:
        {
            std::string lastText = fields[currentTab]->toPlainText().toStdString();
            std::string newText = e->text().toStdString();
            std::string text = lastText + newText;
            fields[currentTab]->setText(QString::fromStdString(text));
        }
        break;
    }
    this->setFocus();
}
