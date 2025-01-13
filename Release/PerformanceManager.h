#ifndef PERFORMANCEMANAGER_H
#define PERFORMANCEMANAGER_H

#include "General.h"
#include <string>
#include <stack>

class PerformanceManager
{
private:
    static int cast16(Graph* G, std::stack<uint>& args);
    static int cast17(Graph* G, uint _in, uint _out);

    static int addV(std::string argv, Graph* G);
    static int addE(std::string argv, Graph* G);

    static int limitlessArgOp(int code, const std::string& argv, Graph* G);
    static int threeArgOp(int code, const std::string& argv, Graph* G);
    static int twoArgOp(int code, const std::string& argv, Graph* G);
    static int oneArgOp(int code, const std::string& argv, Graph* G);
    static int nonArgOp(int code, const std::string& argv, Graph* G);

public:
    PerformanceManager();
    ~PerformanceManager();

    static int operation(int code, int argc, const std::string& argv, Graph* G);

};

#endif // PERFORMANCEMANAGER_H
