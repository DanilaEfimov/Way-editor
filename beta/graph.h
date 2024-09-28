#ifndef GRAPH_H
#define GRAPH_H

#include<map>
#include<set>
#include<string>
#include<QString>

typedef unsigned int uint;
typedef std::pair<unsigned int, unsigned int> edge;

enum call{ // names of functions & their id
    addV = 0x01, addE, eraseV, eraseE,
    getEulerCycle = 0x10, getCycleBase
};

class Graph
{
private:
    uint V; // vertexes
    uint E; // edges
    static uint id;

    bool** connectivityMat;
    std::map<uint, std::set<uint>> connectivityList;
    std::set<edge> edgeList;

    void initConnectivityMat(std::string path);
    void initConnectivityList(std::string path);
    void initEdgeList(std::string path);

    void initByMat();
    void initByEL(); // edges list
    void initByVL(); // vertexes list

    void defaultSettings();
public:
    Graph(std::string path = "");
    virtual ~Graph();

    // Accessors
    uint getID() const;
    QString show(bool fileFlag = false) const;

    // Math
    int addVertex(std::vector<uint>& list);
    int addEdge(edge& e);
    int eraseVertex(uint id);
    int eraseEdge(edge& e);

};

#endif // GRAPH_H
