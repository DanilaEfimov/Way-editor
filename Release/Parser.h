#ifndef PARSER_H
#define PARSER_H

#include "Graphs/Graph.h"
#include <QString>

typedef unsigned char byte, byte_t;
typedef unsigned short ushort;

class Parser
{
private:
    static std::string answer;
public:
    Parser();

    static const std::string getTempDirectory(const std::string& path);
    static const std::string sType(uint code);
    static uint getType(const std::string& cmd);
    static ushort getVertexCount(int fileType,const std::string& path);

    // parsing graph's file
    static int getExtention(std::string fileName);
    static byte** writeMatrixMat(ushort V, const std::string& path);
    static byte** writeMatrixVL (ushort V, const std::string& path);
    static byte** writeMatrixEL (ushort V, const std::string& path);
    static byte** initMatrix(int fileType, const std::string& path);

    // present graph into file
    static void graphToMat(Graph* G, std::fstream& file);
    static void graphToVL(Graph* G, std::fstream& file);
    static void graphToEL(Graph* G, std::fstream& file);

    // command line
    static bool lastLineIsEmpty(uint index);
    static QString getLastLine(uint index);
    static int commandCode(const std::string& command);
    static int argc(int commandCode);
    static std::string argv(const std::string& cmd);

    // functions ::PerformanceMananger
    static int argc(const std::string& argv);

    // saving file text
    static std::string rewriteMat(ushort V, byte** mat);
    static std::string rewriteVL(ushort V,byte** mat);
    static std::string rewriteEL(ushort V,byte** mat);
    static std::string readeableGraph(Graph* G);

    // init Graph
    static Graph* initGraph(int graphType, ushort V, byte** mat);
};

#endif // PARSER_H
