#include"graph.h"
#include"General.h"

#include<fstream>
#include<sstream>
#include<QMessageBox>
#include<QDateTime>

uint Graph::id = 0;

bool isValidPath(const std::string& path){
    if(path == ""){
        errorMassege(EMPTY_PATH);
        return false;
    }
    return true;
}

bool isOKFile(const std::fstream& initFile){
    if(!initFile.is_open()){
        errorMassege(FAILED_TO_OPEN);
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
    uint item = -1;
    while(!initFile.eof() && item) {
        item = 0;
        std::getline(initFile, tempS);
        vertexCounter++;
        std::stringstream ss(tempS);
        while(!ss.eof()){
            ss >> item;
            if(item > 0) {
                if(item > this->V)
                    this->V = item;
                this->connectivityList[vertexCounter].insert(item);
                this->E++;
            }
            else{
                break;
            }
        }
    }
    vertexCounter--;

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
        errorMassege(EMPTY_PATH);
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

std::string Graph::execution(const std::string& cmd) {
    std::string callName = CMD_FLAG;
    std::stringstream tempCmd(cmd);
    tempCmd >> callName;

    int Id = -1;
    if(calls.find(callName) != calls.end()){
        Id = calls.find(callName)->second;
    }
    else{
        errorMassege(UNDEFINED_CMD);
        return (std::string&)"";
    }

    // here in own case reading parameters and turns into command(...)
    std::string parameters = CMD_FLAG;
    tempCmd >> parameters;
    return this->command(parameters, Id);
}

std::string Graph::command(const std::string &parameters, int code) {
    std::stringstream ss(parameters, std::ios_base::ate);
    std::string answer = CMD_FLAG;
    switch(code){
    case weigh:
        break;
    case addV:  // addV 1 2 3 4 ...
        if(true){
            std::vector<uint> list{};
            while(!ss.eof()){
                int vert = 0;
                ss >> vert;
                list.push_back(vert);
            }
            this->addVertex(list);
        }
        break;  // here we don't need answer
    case addE:  // addE 2 3 2 4 2 5 [it means (2,3), (2,4), (2,5) edges]
        if(true){
            std::vector<edge> list{};
            while(!ss.eof()){
                int begin = 0;
                int end = 0;
                ss >> begin; // edge (begin, end)
                if(!ss.eof()){
                    ss >> end;
                }
                else {
                    errorMassege(INVALID_ARGUMENT);
                    return INVALID_ARGUMENT;
                }
                list.push_back(edge(begin, end));
            }
            this->addEdge(list);
        }
        break;  // here we don't need answer
    case eraseV:    // eraseV 'index' [ eraseV 4 e.g. ]
        if(true){
            std::vector<uint> list{};
            while(!ss.eof()){
                int vert = 0;
                ss >> vert;
                list.push_back(vert);
            }
            this->addVertex(list);
        }
        break;
    case eraseE:
        if(true){
            std::vector<edge> list{};
            while(!ss.eof()){
                int begin = 0;
                int end = 0;
                ss >> begin; // edge (begin, end)
                if(!ss.eof()){
                    ss >> end;
                }
                else {
                    errorMassege(INVALID_ARGUMENT);
                    return INVALID_ARGUMENT;
                }
                list.push_back(edge(begin, end));
            }
            this->eraseEdge(list);
        }
        break;
    case EulerCycle:
        break;
    case CycleBase:
        break;
    case Blocks:
        break;
    case MaxV:
        break;
    case MaxE:
        break;
    case Degree:
        break;
    case VW:
        break;
    case EW:
        break;
    case computeDFS:
        break;
    case computeBFS:
        break;
    case computePrima:
        break;
    default:
        undefinedError();
        return CMD_FLAG;
        break;
    }

    return answer;
}

uint Graph::getID() const {
    return id;
}

QString Graph::show(const std::string& path) const {
    QString res = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy") + '\n';
    std::string tempRes = "system " + itos(this->getID()) + '\n';
    for(auto& l : this->connectivityList){
        tempRes += itos(l.first);
        tempRes += ")\t";
        for(auto& n : l.second){
            tempRes += itos(n);
            if(++l.second.find(n) != l.second.end()) {
                tempRes += ", ";
            }
        }
        tempRes += '\n';
    }
    res += tempRes.c_str();

    if(path != ""){
        std::fstream file(path, std::ios::app);
        if(!file.is_open()){
            QMessageBox fail;
            fail.setInformativeText(FAILED_TO_OPEN);
            fail.setWindowTitle("Fail");
            fail.exec();
        }

        file << "count of vertexes:\t" << this->V << '\n';
        file << "count of edges:\t\t"  << this->E << '\n';

        this->showVL(file);
        this->showEL(file);
        this->showMat(file);

        file.close();
    }
    return res;
}

void Graph::showVL(std::fstream &in) const {
    if(in.good()){
        in << "Vertexes connectivity list:\n";
        for(auto& l : this->connectivityList){
            in << l.first << ')' << '\t';
            for(auto& v : l.second){
                in << v << ' ';
            }
            in << std::endl;
        }
    }
    else{
        undefinedError();
    }
}

void Graph::showEL(std::fstream &in) const {
    if(in.good()){
        in << "Edges list:\n";
        for(auto& e : this->edgeList){
            in << '(' << e.first << ',' << ' ' << e.second << ')' << '\n';
        }
    }
    else{
        undefinedError();
    }
}

void Graph::showMat(std::fstream &in) const {
    if(in.good()){
        in << "Matrix of connectivity:\n";
        for(uint i = 0; i < this->V; i++){
            for(uint j = 0; j < this->V; j++){
                in << (1 ? this->connectivityMat[i][j] : 0) << ' ';
            }
            in << '\n';
        }
        in << '\n';
    }
    else{
        undefinedError();
    }
}

// ================ MATH ================

inline int Graph::addVertex(std::vector<uint> &list) {
    for(uint& v : list){
        if(v <= V &&
            this->connectivityList.find(v) != this->connectivityList.end()){
            this->E++;
            this->connectivityList[v].insert(V);
        }
    }
    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->V++;
    this->initByVL();

    return addV;
}

inline int Graph::addEdge(std::vector<edge>& e) {
    for(auto& ed : e){
        if(this->connectivityList.find(ed.first) == this->connectivityList.end() ||
            this->connectivityList.find(ed.first) == this->connectivityList.end()){
            return errorMassege(UNDEFINED_VERTEX);
        }
        this->edgeList.insert(ed);
        this->E++;
    }

    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->initByEL();

    return addE;
}

inline int Graph::eraseVertex(std::vector<uint>& _id) {
    for(auto& vert : _id){
        if(this->connectivityList.find(vert) == this->connectivityList.end()){
            return errorMassege(UNDEFINED_VERTEX);
        }
        this->connectivityList.erase(vert);
    }

    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->V--;
    this->initByVL();

    return eraseV;
}

inline int Graph::eraseEdge(std::vector<edge>& e) {
    for(auto& ed : e){
        if(this->edgeList.find(ed) == this->edgeList.end()){
            return errorMassege(UNDEFINED_EDGE);
        }
        this->edgeList.erase(ed);
        this->E--;
    }

    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->initByEL();

    return eraseE;
}

int Graph::weightGraph(bool isVertex, uint mode) {
    int res = isVertex ? 0x10000000 : 0x00000000;
    if(isVertex){
        switch(mode){
        case defaultV:
            this->defaultVWeight();
            res += defaultV;
            break;
        case degreeV:
            this->degreeVWeight();
            res += degreeV;
            break;
        case normalizeV:
            this->normalizeVWeight();
            res += normalizeV;
            break;
        }
    }
    else {
        switch(mode){
        case defaultE:
            this->defaultEWeight();
            res += defaultE;
            break;
        case degreeE:
            this->degreeEWeight();
            res += degreeE;
            break;
        case normalizeE:
            this->normalizeEWeight();
            res += normalizeE;
            break;
        }
    }
    return res;
    // res can be:
    // 0x10000002, 0x10000001, 0x10000000
    // 0x00000002, 0x00000001, 0x00000000
}

//  ================ WEIGHT_MODES ================

inline void Graph::defaultVWeight() {
    this->normalizeVWeight();
    for(auto& w : this->vertexWeights){ // O(V)
        uint v = w.first;    // vertex id
        float weight = 0.0f;
        for(auto& t : this->connectivityList[v]){
            weight += this->vertexWeights[t];
        }
        if(this->connectivityList[v].size()){
            weight /= this->connectivityList[v].size();
            this->vertexWeights[v] = weight;
        }
        else{
            undefinedError();
        }
    }
}

void Graph::degreeVWeight() {
    for(auto& v : this->connectivityList){
        this->vertexWeights[v.first] = (float)v.second.size();
    }
}

void Graph::normalizeVWeight() {
    for(auto& v : this->connectivityList){
        this->vertexWeights[v.first] = 1.0f;
    }
}

inline void Graph::defaultEWeight() {
    for(auto& e : this->edgeList){
        uint beginPower = this->connectivityList[e.first].size();
        uint endPower = this->connectivityList[e.second].size();
        this->edgeWeights[e] = beginPower > endPower ? 1 : 0;
    }
    // it means that edge directed from stronger vertex to weaker it's FALSE = 0
    // if edge directed reverse it's TRUE = 1
}

inline void Graph::degreeEWeight() {
    this->normalizeEWeight();
    for(auto& w : this->edgeWeights){ // O(V)
        edge e = w.first;    // vertex id
        float weight = 0.0f;
        weight += this->connectivityList[e.first].size();   // degree of begin vertex
        weight += this->connectivityList[e.second].size();   // degree of end vertex
        weight /= 2;
        this->edgeWeights[e] = weight;
    }
}

void Graph::normalizeEWeight()
{

}


