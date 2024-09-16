
#include "menubar.h"

QMenu** MenuBar::menues = nullptr;
QAction** MenuBar::actions = nullptr;
QMenuBar* MenuBar::bar = nullptr;

MenuBar::MenuBar() {
    this->setVisible(true);

    initActions();
    initMenues();
    bindActions();
    bindMenues();
}

MenuBar::~MenuBar() {
    deleteActions();
    deleteMenues();
    delete bar;
}

QMenuBar *MenuBar::getMenuBar() {
    return bar;
}

QMenu *MenuBar::getMenu(uint index) {
    if(index >= menuc){
        return nullptr;
    }
    else{
        return menues[index];
    }
}

QAction *MenuBar::getAction(uint index) {
    if(index >= actc){
        return nullptr;
    }
    else{
        return actions[index];
    }
}

void MenuBar::initActions()
{
    actions = new QAction*[actc];
    for(uint i = 0; i < actc; i++){
        actions[i] = new QAction;
    }
    // set names
    actions[0]->setText("New Session"); // for file menu
    actions[1]->setText("Save");
    actions[2]->setText("White theme"); // for view menu
    actions[3]->setText("Black theme");
    actions[4]->setText("Right mode");
    actions[5]->setText("Left mode");
    actions[6]->setText("Help");        // for line menu
    actions[7]->setText("Open line");
    actions[8]->setText("History");
}

void MenuBar::initMenues()
{
    menues = new QMenu*[menuc];
    for(uint i = 0; i < menuc; i++){
        menues[i] = new QMenu;
    }
    // set names
    menues[0]->setTitle("File");
    menues[1]->setTitle("View");
    menues[2]->setTitle("Command line");
}

void MenuBar::bindActions()
{
    menues[0]->addAction(actions[0]);   // file menu
    menues[0]->addAction(actions[1]);
    menues[1]->addAction(actions[2]);   // view menu
    menues[1]->addAction(actions[3]);
    menues[1]->addAction(actions[4]);
    menues[1]->addAction(actions[5]);
    menues[2]->addAction(actions[6]);   // line menu
    menues[2]->addAction(actions[7]);
    menues[2]->addAction(actions[8]);
}

void MenuBar::bindMenues()
{
    bar = new QMenuBar;
    for(uint i = 0; i < menuc; i++){
        bar->addMenu(menues[i]);
    }
}

void MenuBar::deleteActions() {
    for(uint i = 0; i < actc; i++){
        delete actions[i];
    }
    delete[] actions;
}

void MenuBar::deleteMenues() {
    for(uint i = 0; i < menuc; i++){
        delete menues[i];
    }
    delete[] menues;
}
