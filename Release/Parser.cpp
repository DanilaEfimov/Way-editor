#include "Parser.h"
#include "Error.h"
#include "Graphs/WTree.h"
#include "Graphs/DPseudoGraph.h"
#include "Graphs/UDWGraph.h"
#include "Graphs/UPseudoGraph.h"
#include "Graphs/WDGraph.h"
#include "Graphs/BiTree.h"
#include<fstream>
#include<sstream>

Parser::Parser() {}

Parser::~Parser() {}

uint Parser::getType(std::string path) {
    std::fstream file(path);
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
    ushort i = 0;
    while(!file.eof()){
        std::getline(file, line);
        std::stringstream ss(line);
        char corner;
        ss >> corner;
        if(corner != '0' && corner != '1'){ continue; }
        ushort j = 0;
        while(!ss.eof() && j != V){
            byte temp;
            ss >> temp;
            temp = temp == '0' ? 0b00000000 : 0b00000001;
            mat[i][j] = temp;
            j++;
        }
        if(j != V){ return nullptr; }
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
    ushort i = 0;
    std::getline(file, line);   // ignore graph type string
    while(!file.eof() && i < V){
        std::getline(file, line);
        std::stringstream ss(line);
        ushort j = 0;
        while(!ss.eof()){
            ss >> j;
            if(j > V){
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
