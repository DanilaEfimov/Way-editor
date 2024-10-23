#ifndef GRAPH_H
#define GRAPH_H

#include"General.h"

class Graph
{
private:
    uint V; // vertexes
    uint E; // edges
    static uint id;

    bool** connectivityMat;
    std::map<uint, std::set<uint>> connectivityList;
    std::map<uint, double> vertexWeights;
    std::map<edge, double> edgeWeights;
    std::set<edge> edgeList;

    void initConnectivityMat(std::string path);
    void initConnectivityList(std::string path);
    void initEdgeList(std::string path);

    void initByMat();// matrix
    void initByEL(); // edges list
    void initByVL(); // vertexes list

    void defaultSettings(); // empty graph
public:
    Graph(std::string path = "");
    virtual ~Graph();

    int parsing(std::string& cmd) const;
    int command(int code = -1);

    // Accessors
    uint getID() const;
    QString show(const std::string& path = "") const;
    void showVL(std::fstream& in) const;
    void showEL(std::fstream& in) const;
    void showMat(std::fstream& in)const;

    // Math
    int weightGraph(bool isVertex = true, uint mode = 0);

    int addVertex(std::vector<uint>& list);
    int addEdge(edge& e);
    int eraseVertex(uint id);
    int eraseEdge(edge& e);

    std::stack<uint> getEulerCycle(uint start = 0) const;
    cycleBase getcycleBase() const;
    blocks getBlocks() const;
    uint getMaxV() const;
    edge getMaxE() const;
    uint getDegree(uint v = 0) const;
    double getWeight() const;

    void setVW(uint v = 0);
    void setEW(edge e);

    tree computeDFSTree(uint root = 0) const;
    tree computeBFSTree(uint root = 0) const;
    tree computePrimaTree(uint root = 0) const;
};

#endif // GRAPH_H
