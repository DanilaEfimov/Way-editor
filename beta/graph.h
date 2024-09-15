#ifndef GRAPH_H
#define GRAPH_H

#include<map>
#include<set>
#include<string>

typedef unsigned int uint;
typedef std::pair<unsigned int, unsigned int> edge;

class Graph
{
private:
    uint V; // vertexes
    uint E; // edges

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
};

#endif // GRAPH_H
