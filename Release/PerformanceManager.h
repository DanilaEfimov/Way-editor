#ifndef PERFORMANCEMANAGER_H
#define PERFORMANCEMANAGER_H

#include "General.h"
#include "Error.h"
#include <string>
#include <sstream>
#include <stack>

class PerformanceManager
{
private:
    template<typename graph>
    static int addV(std::string argv, graph* G){
        std::stringstream ss(argv);
        std::stack<uint> args;
        while(!ss.eof()){
            int v = 0;
            ss >> v;
            if(v == 0 || v > G->getV()) {
                Error(_INVALID_ARGUMENT_);
                return -1;
            }
            args.push(v);
        }
        G->operator+(args);
        return functions::addV;
    }

    template<typename graph>
    static int addE(std::string argv, graph* G){
        std::stringstream ss(argv);
        while(!ss.eof()){
            int in = 0, out = 0;
            ss >> in >> out;
            if(in <= out || in == 0 || out == 0){
                Error(__UNDEFINED_BAHAVIOUR__, true);
            }
            G->setEdge(in, out);
        }
    }

    template<typename graph>
    static int limitlessArgOp(int code, const std::string& argv, graph* G){
        int res = 0;
        switch(code){
        case functions::addV: PerformanceManager::addV(argv, G); break;
        case functions::addE: PerformanceManager::addE(argv, G); break;
        case functions::eraseV: break;
        case functions::eraseE: break;
        default: return -1; break;
        }
        return res;
    }

public:
    PerformanceManager();
    ~PerformanceManager();

    template<typename graph>
    static int operation(int code, int argc, const std::string& argv, graph* G){
        int res = 0;
        switch(argc){
        case ZERO: break;
        case ONE: break;
        case TWO: break;
        case THREE: break;
        case LIMITLESS: res = limitlessArgOp(code, argv, G); break;
        default:
            return -1;
            break;
        }
        return res;
    }

};

#endif // PERFORMANCEMANAGER_H
