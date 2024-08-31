#ifndef GRAPH_H
#define GRAPH_H

#include<string>
#include<set>
#include<map>

typedef unsigned int uint, id;

class Graph
{
private:
    static uint count;
    uint ID;
    uint V;
    uint E;

    std::set<uint> vertexes;
    std::map<id, std::set<uint>> connectivityList;
    bool** connectivityMat;
public:
    Graph();
    Graph(std::string path);
    ~Graph();

    float connectivity();

    void addVertex();

    // Accessors
    uint* getID();
    uint* getV();
    uint* getE();

    std::set<uint>* getVertexes();
    std::map<id, std::set<uint>>* getConnectivityList();
    bool*** getConnectivityMat();
};

#endif // GRAPH_H
