#ifndef PERFORMANCEMANAGER_H
#define PERFORMANCEMANAGER_H

#include "General.h"
#include <string>
#include <QString>
#include <stack>

class PerformanceManager
{
private:
    static QString nonVoidAnswer;

    static int cast16(Graph* G, std::stack<uint>& args);
    static int cast17(Graph* G, uint _in, uint _out);
    static int cast18(Graph* G, uint v);
    static int cast19(Graph* G, uint _in, uint _out);

    static int cast37(Graph* G, uint v);

    static int addV(const std::string& argv, Graph* G);
    static int addE(const std::string& argv, Graph* G);
    static int eraseV(const std::string& argv, Graph* G);
    static int eraseE(const std::string& argv, Graph* G);

    static int degree(const std::string& argv, Graph* G);

    static int limitlessArgOp(int code, const std::string& argv, Graph* G);
    static int threeArgOp(int code, const std::string& argv, Graph* G);
    static int twoArgOp(int code, const std::string& argv, Graph* G);
    static int oneArgOp(int code, const std::string& argv, Graph* G);
    static int nonArgOp(int code, const std::string& argv, Graph* G);

public:
    PerformanceManager();
    ~PerformanceManager();

    static int operation(int code, int argc, const std::string& argv, Graph* G);
    static bool isDirected(int code);
    static bool isPseudo(int code);
    static bool isVoidOp(int code);

    // ACCESSORS
    static QString& getAnswer();

};

// how to return non void operations?
// in parser make function for converting rather return types to string
// here make static member string which contains last answer.
// than ### IN MAINWINDOW :: CmdEvent ### if res == 0 && !isVoidOp =>
// => add answer string (static member) into input area.

#endif // PERFORMANCEMANAGER_H
