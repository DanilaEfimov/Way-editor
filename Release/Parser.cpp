#include "Parser.h"
#include "Error.h"
#include "General.h"
#include "Graphs/WTree.h"
#include "Graphs/DPseudoGraph.h"
#include "Graphs/UDWGraph.h"
#include "Graphs/UPseudoGraph.h"
#include "Graphs/WDGraph.h"
#include "Graphs/BiTree.h"
#include "mainwindow.h"
#include <fstream>
#include <sstream>

std::string Parser::answer = "";

Parser::Parser() {}

uint Parser::getType(std::string cmd) {
    std::fstream file(cmd);
    std::string typeName = "";
    std::getline(file, typeName);
    if(types.find(typeName) == types.end()){
        file.close();
        return -1;
    }
    else {
        file.close();
        int type = types.find(typeName)->second;
        return type;
    }
    file.close();
    return -1;
}

ushort Parser::getVertexCount(int fileType, std::string path) {
    ushort V = 0;
    std::fstream file(path);
    std::string line = "";
    switch(fileType){
    case MAT:   // look at .mat descriptor
        std::getline(file, line);
        file >> V;
        break;
    case VL:    // here parser searching greatest number of all file
        std::getline(file, line);   // first line is type of graph
        while(!file.eof()){
            ushort temp = 0;
            std::getline(file, line);
            std::stringstream ss(line);
            while(!ss.eof()){
                ss >> temp;
                V = temp > V ? temp : V;
            }
        }
        break;
    case EL:    // here parser searching greatest number of all file
        std::getline(file, line); // first line is graph type
        while(!file.eof()){
            ushort temp = 0;
            std::getline(file, line);
            std::stringstream ss(line);
            while(!ss.eof()){
                ss >> temp;
                V = temp > V ? temp : V;
            }
        }
        break;
    default:
        break;
    }
    file.close();
    return V;
}

int Parser::getExtention(std::string fileName) {
    if(fileName.size() == 0){
        Error(_UNCORRECT_FILE_NAME_);
        return -1;
    }
    size_t pos = fileName.find_last_of('.');
    size_t size = fileName.size();
    std::string extention = "";
    for(size_t i = pos; i < size; i++){
        extention += fileName[i];
    }
    if(extentions.find(extention) == extentions.end()){
        return -1;
    }
    std::map<std::string, int>::const_iterator it = extentions.find(extention);
    extKey res = *it;
    return res.second;
}

byte** Parser::writeMatrixMat(ushort V, std::string path) {
    if(V == 0){ return nullptr; }
    byte** mat = new byte*[V];
    for(size_t i = 0; i < V; i++){
        mat[i] = new byte[V]{0};
    }
    std::fstream file(path);
    std::string line = "";
    std::getline(file, line);   // ignore type of graph
    std::getline(file, line);   // ignore count of vertexes
    ushort i = 0;
    bool mistake = false;
    while(!file.eof() && i != V){
        std::getline(file, line);
        std::stringstream ss(line);
        ushort j = 0;
        while(!ss.eof() && j != V){
            ushort item = 0;
            ss >> item;
            if(item > 1 && !mistake){
                mistake = true;
                Error(_UNCORRECT_FILE_NAME_, true);
                break;
            }
            mat[i][j] = item;
            j++;
        }
        i++;
    }
    if(i != V){
        Error(_UNCORRECT_FILE_NAME_);
        file.close();
        return nullptr;
    }
    file.close();
    return mat;
}

byte** Parser::writeMatrixVL(ushort V, std::string path) {  // NOT fixed
    byte** mat = nullptr;
    mat = new byte*[V];
    for(size_t i = 0; i < V; i++){
        mat[i] = new byte[V]{0};
    }
    std::fstream file(path);
    std::string line = "";
    std::getline(file, line);   // ignore graph type string
    ushort i = 0;
    bool mistake = false;
    while(!file.eof() && i < V){
        std::getline(file, line);
        std::stringstream ss(line);
        ushort j = 0;
        while(!ss.eof()){
            ss >> j;
            if(j > V && !mistake){
                mistake = true;
                Error(__UNCORRECT_INPUT_FILE__, true);
                continue;
            }
            mat[i][j-1] = 0b00000001;
        }
        i++;
    }
    file.close();
    return mat;
}

byte** Parser::writeMatrixEL(ushort V, std::string path) {
    byte** mat = new byte*[V];
    for(size_t i = 0; i < V; i++){
        mat[i] = new byte[V]{0};
    }
    std::fstream file(path);
    if(!file.is_open()){
        Error(_FAILED_TO_OPEN_);
        file.close();
        return mat;
    }
    std::string line = "";
    while(!file.eof()){
        std::getline(file, line);
        std::stringstream ss(line);
        ushort _out;
        ushort _in;
        ss >> _out >> _in;
        mat[_out - 1][_in - 1] = 0b00000001;
    }
    file.close();
    return mat;
}

byte** Parser::initMatrix(int fileType, std::string path) {
    byte** mat = nullptr;
    ushort V = getVertexCount(fileType, path);
    if(fileType == -1){
        Error(_ERROR_FILE_TYPE_);
        return mat;
    }
    switch(fileType){
    case MAT:
        mat = writeMatrixMat(V, path);
        break;
    case VL:
        mat = writeMatrixVL(V, path);
        break;
    case EL:
        mat = writeMatrixEL(V, path);
        break;
    default:
        return nullptr;
    }
    return mat;
}

bool Parser::lastLineIsEmpty(uint index) {
    QTextEdit* current = MainWindow::fields[index];
    QString line = current->toPlainText();
    std::string lastLine = "";
    std::stringstream ss(line.toStdString());
    while(!ss.eof()){
        std::getline(ss, lastLine);
    }
    return lastLine.size() == 0 || lastLine == "";
}

QString Parser::getLastLine(uint index) {
    QTextEdit* current = MainWindow::fields[index];
    QString line = current->toPlainText();
    std::string lastLine = "";
    std::stringstream ss(line.toStdString());
    while(!ss.eof()){
        std::getline(ss, lastLine);
    }
    return QString::fromStdString(lastLine);
}

int Parser::commandCode(std::string command) {
    std::string functionName;
    size_t left = 0, right = command.find_first_of(' ');
    functionName = command.substr(left, right);
    if(commands.find(functionName) == commands.end()){
        Error(__SYNTAX_ERROR__, true);
        return -1;
    }
    else{
        int _code = (commands.find(functionName))->second;
        return _code;
    }
}

int Parser::argc(int commandCode) {
    if(argcount.find(commandCode) == argcount.end()){
        Error(__SYNTAX_ERROR__, true);
        return -1;
    }
    else{
        int _code = (argcount.find(commandCode))->second;
        return _code;
    }
}

std::string Parser::argv(std::string cmd) {
    std::string argv;
    size_t left = cmd.find_first_of(' '), right = cmd.size() - 1;
    argv = cmd.substr(left, right);
    return argv;
}

std::string Parser::rewriteMat(ushort V, byte** mat) {
    if(mat == nullptr){
        Error(_UNDEFINED_ERROR_);
        return "";
    }
    answer += "\n";
    answer = std::to_string(V);
    answer += "\n";
    for(size_t i = 0; i < V; i++){
        for(size_t j = 0; j < V; j++){
            answer += mat[i][j] ? "1 " : "0 ";
        }
        answer += "\n";
    }
    answer += _MAT_HELLO_;
    return answer;
}

std::string Parser::rewriteVL(ushort V, byte** mat) {
    if(mat == nullptr){
        Error(_UNDEFINED_ERROR_);
        return "";
    }
    answer = std::to_string(V);
    answer += "\n";
    for(size_t i = 0; i < V; i++){
        for(size_t j = 0; j < V; j++){
            std::string id = std::to_string(j + 1) + " ";
            if(mat[i][j]){
                answer += id;
            }
        }
        answer += "\n";
    }
    return answer;
}

std::string Parser::rewriteEL(ushort V, byte** mat) {
    if(mat == nullptr){
        Error(_UNDEFINED_ERROR_);
        return "";
    }
    answer += "\n";
    for(size_t i = 0; i < V; i++){
        for(size_t j = 0; j < V; j++){
            std::string id = std::to_string(j + 1) + " ";
            if(mat[i][j]){
                answer += std::to_string(i+1) + " " + std::to_string(j+1) + "\n";
            }
        }
        answer += "\n";
    }
    return answer;
}

std::string Parser::graphType(int code) {
    switch(code){
    case udirgraph: return "UDirGraph";
    case dirgraph: return "DirGraph";
    case udwgraph: return "UDWGraph";
    case wdgraph: return "DWGraph";
    case upseudograph: return "UPseudoGraph";
    case dpseudograph: return "DPseudoGraph";
    case tree: return "Tree";
    case wtree: return "WTree";
    case bitree: return "BiTree";
    default:
        Error(_ERROR_GRAPH_TYPE_);
        return "Graph";
    }
}

std::string Parser::readeableGraph(Graph* G) {
    return graphType(G->getType()) + G->show() + "\n" + _HELLO_;
}

Graph* Parser::initGraph(int graphType, ushort V, byte** mat) {
    switch(graphType){
    case udirgraph: return new UDirGraph(V, mat);
    case dirgraph: return new DirGraph(V, mat);
    case udwgraph: return new UDWGraph(V, mat);
    case wdgraph: return new WDGraph(V, mat);
    case upseudograph: return new UPseudoGraph(V, mat);
    case dpseudograph: return new DPseudoGraph(V, mat);
    case tree: return new Tree(V, mat);
    case wtree: return new WTree(V, mat);
    case bitree: return new BiTree(V, mat);
    default:
        Error(_ERROR_GRAPH_TYPE_);
        return nullptr;
    }
    return nullptr;
}
