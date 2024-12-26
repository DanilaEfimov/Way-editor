#ifndef PARSER_H
#define PARSER_H

#include "General.h"
#include "Graphs/Graph.h"

typedef unsigned char byte, byte_t;
typedef unsigned short ushort;

class Parser
{
private:

public:
    Parser();
    virtual ~Parser();

    static uint getType(std::string path);
    static ushort getVertexCount(int fileType, std::string path);

    // parsing graph's file
    static int getExtention(std::string fileName);
    static byte** writeMatrixMat(ushort V, std::string path);      // for .mat file
    static byte** writeMatrixVL (ushort V, std::string path);
    static byte** writeMatrixEL (ushort V, std::string path);
    static byte** initMatrix(int fileType, std::string path);

    // command line
    static int commandCode(std::string command);
    static int argc(int commandCode);

    // init Graph
    static Graph* initGraph(int graphType, ushort V, byte** mat);
};

#endif // PARSER_H
