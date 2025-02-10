#include "PerformanceManager.h"
#include "Error.h"
#include "Parser.h"
#include <set>
#include <sstream>

QString PerformanceManager::nonVoidAnswer = "";

int PerformanceManager::cast16(Graph *G, std::stack<uint> &args){
    int code = G->getType();
    switch(code){
        case names::udirgraph:      static_cast<UDirGraph*>(G)->operator+(args);    break;
        case names::dirgraph:       static_cast<DirGraph*>(G)->operator+(args);     break;
        case names::udwgraph:       static_cast<UDWGraph*>(G)->operator+(args);     break;
        case names::wdgraph:        static_cast<WDGraph*>(G)->operator+(args);      break;
        case names::upseudograph:   static_cast<UPseudoGraph*>(G)->operator+(args); break;
        case names::dpseudograph:   static_cast<DPseudoGraph*>(G)->operator+(args); break;
        case names::tree:           static_cast<Tree*>(G)->operator+(args);         break;
        case names::wtree:          static_cast<WTree*>(G)->operator+(args);        break;
        case names::bitree:         static_cast<BiTree*>(G)->operator+(args);       break;
    default:
        return -1;
    }
    return 0;
}

int PerformanceManager::cast17(Graph *G, uint _out, uint _in) {
    int code = G->getType();
    switch(code){
        case names::udirgraph:      static_cast<UDirGraph*>(G)->setEdge(_out, _in);     break;
        case names::dirgraph:       static_cast<DirGraph*>(G)->setEdge(_out, _in);      break;
        case names::udwgraph:       static_cast<UDWGraph*>(G)->setEdge(_out, _in);      break;
        case names::wdgraph:        static_cast<WDGraph*>(G)->setEdge(_out, _in);       break;
        case names::upseudograph:   static_cast<UPseudoGraph*>(G)->setEdge(_out, _in);  break;
        case names::dpseudograph:   static_cast<DPseudoGraph*>(G)->setEdge(_out, _in);  break;
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
    default: Error(_UNDEFINED_ERROR_);
        return -1;
    }
    return 0;
}

int PerformanceManager::cast37(Graph *G, uint v) {
    int code = G->getType();
    uint degree = 0;
    switch(code){
        case names::udirgraph:      degree = static_cast<UDirGraph*>(G)->operator()(v);     break;
        case names::dirgraph:       degree = static_cast<DirGraph*>(G)->operator()(v);      break;
        case names::udwgraph:       degree = static_cast<UDWGraph*>(G)->operator()(v);      break;
        case names::wdgraph:        degree = static_cast<WDGraph*>(G)->operator()(v);       break;
        case names::upseudograph:   degree = static_cast<UPseudoGraph*>(G)->operator()(v);  break;
        case names::dpseudograph:   degree = static_cast<DPseudoGraph*>(G)->operator()(v);  break;
        case names::tree:           degree = static_cast<Tree*>(G)->operator()(v);          break;
        case names::wtree:          degree = static_cast<WTree*>(G)->operator()(v);         break;
        case names::bitree:         degree = static_cast<BiTree*>(G)->operator()(v);        break;
    default: Error(_UNDEFINED_ERROR_);
        return -1;
    }
    nonVoidAnswer = "\n" + Parser::intToString(degree);
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
    if(args.size() > G->getV() && args.top() != 0){
        Error(_INVALID_ARGUMENT_COUNT_);
        return -1;
    }
    return cast16(G, args);
}

int PerformanceManager::addE(const std::string &argv, Graph *G){
    int code = 0;
    std::stringstream ss(argv);
    uint type = G->getType();
    bool ispseudo = isPseudo(type);
    bool isdirected = isDirected(type);
    while(!ss.eof()){
        int in = 0, out = 0;
        ss >> out >> in;
        if(in*out == 0 || (in==out && !ispseudo)){
            Error(__UNDEFINED_BAHAVIOUR__, true);
        }
        code = cast17(G, out, in);
        if(code != 0){return code;}
    }
    return 0;
    // input format: argv:: from-vertex [] to-vertex
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
    if(argc % 2){ Error(_INVALID_ARGUMENT_COUNT_); return -1;}
    static std::set<edge_t> vertexes;
    std::stringstream ss(argv);
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

int PerformanceManager::degree(const std::string &argv, Graph *G){
    int code = 0;
    std::stringstream ss(argv);
    int vertex;
    ss >> vertex;
    code = cast37(G, vertex);
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

int PerformanceManager::twoArgOp(int code, const std::string &argv, Graph *G) {
    int res = 0;
    switch(code){
    default: return -1; break;
    }
    return res;
}

int PerformanceManager::oneArgOp(int code, const std::string &argv, Graph *G) {
    int res = 0;
    switch(code){
        case functions::Degree: res = PerformanceManager::degree(argv, G); break;
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
        case ONE: res = oneArgOp(code, argv, G); break;
        case TWO: break;
        case THREE: break;
        case LIMITLESS: res = limitlessArgOp(code, argv, G); break;
    default: return -1; break;
    }
    return res;
}

bool PerformanceManager::isDirected(int code) {
    switch(code){
        case names::udirgraph:      return false;   break;
        case names::dirgraph:       return true;    break;
        case names::udwgraph:       return false;   break;
        case names::wdgraph:        return true;    break;
        case names::upseudograph:   return false;   break;
        case names::dpseudograph:   return true;    break;
        case names::tree:           return false;   break;
        case names::wtree:          return false;   break;
        case names::bitree:         return false;   break;
    default: Error(_ERROR_GRAPH_TYPE_); return false; break;
    }
    return false;
    // ### You can look at all Classes in General.h::names enum. ###
}

bool PerformanceManager::isPseudo(int code) {
    switch(code){
        case names::udirgraph:      return false;   break;
        case names::dirgraph:       return false;   break;
        case names::udwgraph:       return false;   break;
        case names::wdgraph:        return false;   break;
        case names::upseudograph:   return true;    break;
        case names::dpseudograph:   return true;    break;
        case names::tree:           return false;   break;
        case names::wtree:          return false;   break;
        case names::bitree:         return false;   break;
    default: Error(_ERROR_GRAPH_TYPE_); return false; break;
    }
    return false;
}

bool PerformanceManager::isVoidOp(int code) {
    switch(code){
    case functions::clear:          return true; break;
    case functions::weight:         return false; break;
    case functions::addV:           return true; break;
    case functions::addE:           return true; break;
    case functions::eraseV:         return true; break;
    case functions::eraseE:         return true; break;
    case functions::EulerCycle:     return true; break;
    case functions::CycleBase:      return true; break;
    case functions::Blocks:         return true; break;
    case functions::MaxV:           return true; break;
    case functions::MaxE:           return true; break;
    case functions::Degree:         return false; break;
    case functions::Weight:         return true; break;
    case functions::VW:             return true; break;
    case functions::EW:             return true; break;
    case functions::computeDFS:     return true; break;
    case functions::computeBFS:     return true; break;
    case functions::computePrima:   return true; break;
    case functions::Dejcstra:       return true; break;
    default: Error(__INVALID_COMMAND__); return false; break;
    }
    return false;
    // ### You can look at all Classes in General.h::functions enum. ###
}

QString &PerformanceManager::getAnswer(){return nonVoidAnswer;}


