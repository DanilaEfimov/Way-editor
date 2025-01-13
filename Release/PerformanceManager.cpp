#include "PerformanceManager.h"
#include "Error.h"
#include <sstream>

int PerformanceManager::cast16(Graph *G, std::stack<uint> &args){
    int code = G->getType();
    switch(code){
    case names::udirgraph:      static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::dirgraph:       static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::udwgraph:       static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::wdgraph:        static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::upseudograph:   static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::dpseudograph:   static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::tree:           static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::wtree:          static_cast<UDirGraph*>(G)->operator+(args);    break;
    case names::bitree:         static_cast<BiTree*>(G)->operator+(args);       break;
    default:
        return -1;
    }
    return 0;
}

int PerformanceManager::cast17(Graph *G, uint _in, uint _out) {
    int code = G->getType();
    switch(code){
    case names::udirgraph:      static_cast<UDirGraph*>(G)->setEdge(_in, _out);     break;
    case names::dirgraph:       static_cast<UDirGraph*>(G)->setEdge(_in, _out);     break;
    case names::udwgraph:       static_cast<UDirGraph*>(G)->setEdge(_in, _out);     break;
    case names::wdgraph:        static_cast<UDirGraph*>(G)->setEdge(_in, _out);     break;
    case names::upseudograph:   static_cast<UDirGraph*>(G)->setEdge(_in, _out);     break;
    case names::dpseudograph:   static_cast<UDirGraph*>(G)->setEdge(_in, _out);     break;
    case names::tree:           Error(__INVALID_COMMAND__); return-1;               break;
    case names::wtree:          Error(__INVALID_COMMAND__); return-1;               break;
    case names::bitree:         Error(__INVALID_COMMAND__); return-1;               break;
    default:
        return -1;
    }
    return 0;
}

int PerformanceManager::addV(std::string argv, Graph *G){
    std::stringstream ss(argv);
    std::stack<uint> args;
    while(!ss.eof()){
        int v = 0;
        ss >> v;
        if(v > G->getV()) {
            Error(_INVALID_ARGUMENT_);
            return -1;
        }
        args.push(v);
        if(v == 0) {break;}
    }
    return cast16(G, args);
}

int PerformanceManager::addE(std::string argv, Graph *G){
    int code = 0;
    std::stringstream ss(argv);
    while(!ss.eof()){
        int in = 0, out = 0;
        ss >> in >> out;
        if(in <= out || in == 0 || out == 0){
            Error(__UNDEFINED_BAHAVIOUR__, true);
        }
        code = cast17(G, in, out);
        if(code != 0){return - 1;}
    }
    return 0;
}

int PerformanceManager::limitlessArgOp(int code, const std::string &argv, Graph *G){
    int res = 0;
    switch(code){
    case functions::addV: res = PerformanceManager::addV(argv, G); break;
    case functions::addE: res = PerformanceManager::addE(argv, G); break;
    case functions::eraseV: break;
    case functions::eraseE: break;
    default: return -1; break;
    }
    return res;
}

PerformanceManager::PerformanceManager() {}

PerformanceManager::~PerformanceManager() {}

int PerformanceManager::operation(int code, int argc, const std::string &argv, Graph *G){
    int res = 0;
    switch(argc){
    case ZERO: break;
    case ONE: break;
    case TWO: break;
    case THREE: break;
    case LIMITLESS: res = limitlessArgOp(code, argv, G); break;
    default: return -1; break;
    }
    return res;
}


