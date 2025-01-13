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
    static uint getType(std::string cmd);
    static ushort getVertexCount(int fileType, std::string path);

    // parsing graph's file
    static int getExtention(std::string fileName);
    static byte** writeMatrixMat(ushort V, std::string path);
    static byte** writeMatrixVL (ushort V, std::string path);
    static byte** writeMatrixEL (ushort V, std::string path);
    static byte** initMatrix(int fileType, std::string path);

    // command line
    static bool lastLineIsEmpty(uint index);
    static QString getLastLine(uint index);
    static int commandCode(std::string command);
    static int argc(int commandCode);
    static std::string argv(std::string cmd);

    // saving file text
    static std::string rewriteMat(ushort V, byte** mat);
    static std::string rewriteVL(ushort V,byte** mat);
    static std::string rewriteEL(ushort V,byte** mat);
    static std::string graphType(int code);
    static std::string readeableGraph(Graph* G);

    // init Graph
    static Graph* initGraph(int graphType, ushort V, byte** mat);
};

#endif // PARSER_H
