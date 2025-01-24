#include "PerformanceManager.h"
#include "Error.h"
#include "Parser.h"
#include <set>
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
    case names::dirgraph:       static_cast<DirGraph*>(G)->setEdge(_in, _out);      break;
    case names::udwgraph:       static_cast<UDWGraph*>(G)->setEdge(_in, _out);      break;
    case names::wdgraph:        static_cast<WDGraph*>(G)->setEdge(_in, _out);       break;
    case names::upseudograph:   static_cast<UPseudoGraph*>(G)->setEdge(_in, _out);  break;
    case names::dpseudograph:   static_cast<DPseudoGraph*>(G)->setEdge(_in, _out);  break;
    case names::tree:           Error(__INVALID_COMMAND__); return -1;              break;
    case names::wtree:          Error(__INVALID_COMMAND__); return -1;              break;
    case names::bitree:         Error(__INVALID_COMMAND__); return -1;              break;
    default:
        return -1;
    }
    return 0;
}

int PerformanceManager::cast18(Graph *G, uint v) {
    int code = G->getType();
    switch(code){
    case names::udirgraph:      static_cast<UDirGraph*>(G)->operator-(v);       break;
    case names::dirgraph:       static_cast<DirGraph*>(G)->operator-(v);        break;
    case names::udwgraph:       static_cast<UDWGraph*>(G)->operator-(v);        break;
    case names::wdgraph:        static_cast<WDGraph*>(G)->operator-(v);         break;
    case names::upseudograph:   static_cast<UPseudoGraph*>(G)->operator-(v);    break;
    case names::dpseudograph:   static_cast<DPseudoGraph*>(G)->operator-(v);    break;
    case names::tree:           static_cast<Tree*>(G)->operator-(v);            break;
    case names::wtree:          static_cast<WTree*>(G)->operator-(v);           break;
    case names::bitree:         static_cast<BiTree*>(G)->operator-(v);          break;
    default:
        return -1;
    }
    return 0;
}

int PerformanceManager::cast19(Graph *G, uint _in, uint _out) {
    int code = G->getType();
    switch(code){
    case names::udirgraph:      static_cast<UDirGraph*>(G)->eraseEdge(_in, _out);     break;
    case names::dirgraph:       static_cast<DirGraph*>(G)->eraseEdge(_in, _out);      break;
    case names::udwgraph:       static_cast<UDWGraph*>(G)->eraseEdge(_in, _out);      break;
    case names::wdgraph:        static_cast<WDGraph*>(G)->eraseEdge(_in, _out);       break;
    case names::upseudograph:   static_cast<UPseudoGraph*>(G)->eraseEdge(_in, _out);  break;
    case names::dpseudograph:   static_cast<DPseudoGraph*>(G)->eraseEdge(_in, _out);  break;
    case names::tree:           static_cast<Tree*>(G)->eraseEdge(_in, _out);          break;
    case names::wtree:          static_cast<WTree*>(G)->eraseEdge(_in, _out);         break;
    case names::bitree:         static_cast<BiTree*>(G)->eraseEdge(_in, _out);        break;
    default:
        return -1;
    }
    return 0;
}

int PerformanceManager::addV(const std::string &argv, Graph *G){
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

int PerformanceManager::addE(const std::string &argv, Graph *G){
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

int PerformanceManager::eraseV(const std::string &argv, Graph *G) {
    int code = 0;
    int argc = Parser::argc(argv);
    if(argc > G->getV()){Error(_INVALID_ARGUMENT_COUNT_);return -1;}
    static std::set<uint> vertexes;
    vertexes.clear();
    std::stringstream ss(argv);
    for(size_t i = 0; i < argc; i++){
        uint v;
        ss >> v;
        vertexes.insert(v);
    }
    for(std::set<uint>::iterator it = vertexes.begin();
        it != vertexes.end();
        it++){
        code = cast18(G, *it);
        if(code != 0){return code;}
    }
    return code;
}

int PerformanceManager::eraseE(const std::string &argv, Graph *G) {
    int code = 0;
    int argc = Parser::argc(argv);
    static std::string funcName;
    static std::set<edge_t> vertexes;
    std::stringstream ss(argv);
    ss >> funcName;
    for(size_t i = 0; i < argc / 2; i++){
        uint _out, _in;
        ss >> _out >> _in;
        vertexes.insert(edge_t(_in, _out));
    }
    for(auto& e : vertexes){
        code = cast19(G, e.first, e.second);
        if(code != 0){return code;}
    }
    return code;
}

int PerformanceManager::limitlessArgOp(int code, const std::string &argv, Graph *G){
    int res = 0;
    switch(code){
    case functions::addV: res = PerformanceManager::addV(argv, G);      break;
    case functions::addE: res = PerformanceManager::addE(argv, G);      break;
    case functions::eraseV: res = PerformanceManager::eraseV(argv, G);  break;
    case functions::eraseE: res = PerformanceManager::eraseE(argv, G);  break;
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


