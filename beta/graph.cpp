#include"graph.h"
#include"General.h"

#include<fstream>
#include<sstream>
#include<QMessageBox>
#include<QDateTime>

uint Graph::id = 0;

bool isValidPath(const std::string& path){
    if(path == ""){
        QMessageBox fail;
        fail.setInformativeText(EMPTY_PATH);
        fail.setWindowTitle("Fail");
        fail.exec();
        return false;
    }
    return true;
}

bool isOKFile(const std::fstream& initFile){
    if(!initFile.is_open()){
        QMessageBox fail;
        fail.setInformativeText(FAILED_TO_OPEN);
        fail.setWindowTitle("Fail");
        fail.exec();
        return false;
    }
    return true;
}

edge getEdge(std::stringstream& ss){
    uint begin = 0;
    uint end = 0;
    ss >> begin >> end;
    return edge(begin, end);
}

void Graph::initConnectivityMat(std::string path) {

    if(!isValidPath(path))
        return;

    std::fstream initFile(path);
    if(!isOKFile(initFile))
        return;

    initFile >> this->V;
    this->connectivityMat = new bool*[this->V];
    for(uint i = 0; i < this->V; i++){
        this->connectivityMat[i] = new bool[this->V]{false};
    }

    std::string tempS = "";
    for(uint i = 0; !initFile.eof() && i < this->V; i++){
        std::getline(initFile, tempS);
        std::stringstream ss(tempS, std::ios_base::ate);
        for(uint j = 0; j < this->V && !ss.eof(); j++){
            int item = 0;
            ss >> item;
            this->connectivityMat[i][j] = item != 0 ? true : false;
        }
    }

    // here the end of the file is not tracked
    // if user will get uncorrectly initialized graph due to matrix
    // they will not get message box here - error from user input
}

void Graph::initConnectivityList(std::string path) {
    if(!isValidPath(path))
        return;

    std::fstream initFile(path);
    if(!isOKFile(initFile))
        return;

    uint vertexCounter = 0;
    this->V = 0;
    this->E = 0;
    std::string tempS = "";
    while(!initFile.eof()) {
        std::getline(initFile, tempS);
        vertexCounter++;
        std::stringstream ss(tempS);
        while(!ss.eof()){
            uint item = 0;
            ss >> item;
            if(item > this->V)
                this->V = item;
            this->connectivityList[vertexCounter].insert(item);
            if(item)
                this->E++;
        }
    }

    if(vertexCounter != this->V)
        undefinedError();

    // V must be equals max index of vertexes
    // because graph is connected and every vertex
    // have even one neighbor, so even
    // max index vertex is writed at file as some vertex's neighbor
}

void Graph::initEdgeList(std::string path) {
    if(!isValidPath(path))
        return;

    std::fstream initFile(path);
    if(!isOKFile(initFile))
        return;

    this->V = 0;
    this->E = 0;
    std::string tempS = "";
    while(!initFile.eof()){
        std::getline(initFile, tempS);
        std::stringstream ss(tempS, std::ios_base::ate);
        for(uint j = 0; j < this->V && !ss.eof(); j++){
            uint item = 0;
            ss >> item;
            if(item > this->V)
                this->V = item;
            this->edgeList.insert(getEdge(ss));
            this->E++;
        }
    }

    if(this->edgeList.size() != this->E)
        undefinedError();

    // such .el file looks like E strings by two nums
    // and again, if in file was not connected graph
    // it won't be detected here and probably will be
    // tracked as "invalid input"/"invalid argument"/... then
}

void Graph::initByMat() { // before calling matrix must be initialized
    for(uint i = 0; i < this->V; i++){
        for(uint j = 0; j < this->V; j++){
            if(this->connectivityMat[i][j]){
                this->edgeList.insert(edge(i+1, j+1));
                this->connectivityList[i+1].insert(j+1);
            }
        }
    }
}

void Graph::initByEL() {
    this->connectivityMat = new bool*[this->V];
    for(uint i = 0; i < this->V; i++){
        this->connectivityMat[i] = new bool[this->V]{false};
    }

    for(edge e : this->edgeList){
        this->connectivityMat[e.first-1][e.second-1] = true;
        this->connectivityList[e.first].insert(e.second);
    }
}

void Graph::initByVL() {
    this->connectivityMat = new bool*[this->V];
    for(uint i = 0; i < this->V; i++){
        this->connectivityMat[i] = new bool[this->V]{false};
    }

    for(auto& l : this->connectivityList){
        for(uint v : l.second){
            this->connectivityMat[l.first-1][v-1] = true;
            this->edgeList.insert(edge(l.first, v));
        }
    }
}

void Graph::defaultSettings() {
    this->V = 0;
    this->E = 0;

    this->connectivityMat = nullptr;
    this->connectivityList = {};
    this->edgeList = {};
}

Graph::Graph(std::string path) {
    id++;
    switch(getExtension(path)){
    case mat:               // file with matrix
        this->initConnectivityMat(path);
        this->initByMat();
        break;
    case edgesList:         // file with list of edges
        this->initEdgeList(path);
        this->initByEL();
        break;
    case vertexesList:      // file with lists of every vertex's neighbors
        this->initConnectivityList(path);
        this->initByVL();
        break;
    default:                 // undefined file
        this->defaultSettings();
        break;
    }
}

Graph::~Graph() {
    id--;
    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;
}

uint Graph::getID() const {
    return id;
}

QString Graph::show() const {
    QString res = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy") + '\n';
    std::string tempRes = "system " + itos(this->getID()) + '\n';
    for(auto l : this->connectivityList){
        tempRes += itos(l.first);
        tempRes += ")\t";
        for(auto n : l.second){
            tempRes += itos(n);
            if(++l.second.find(n) != l.second.end()) {
                tempRes += ", ";
            }
        }
        tempRes += '\n';
    }
    res += tempRes.c_str();

    return res;
}
