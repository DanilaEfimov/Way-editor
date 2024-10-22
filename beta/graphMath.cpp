#ifndef GRAPHMATH_H
#define GRAPHMATH_H

#include"graph.h"
#include"General.h"

inline int Graph::addVertex(std::vector<uint> &list) {
    this->V++;
    for(uint& v : list){
        if(v <= V &&
            this->connectivityList.find(v) != this->connectivityList.end()){
            this->E++;
            this->connectivityList[v].insert(V);
        }
    }
    for(uint i = 0; i < this->V - 1; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->initByVL();

    return addV;
}

inline int Graph::addEdge(edge &e) {
    if(this->connectivityList.find(e.first) == this->connectivityList.end() ||
        this->connectivityList.find(e.first) == this->connectivityList.end()){
        return errorMassege(UNDEFINED_VERTEX);
    }
    this->edgeList.insert(e);
    this->E++;

    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->initByEL();

    return addE;
}

inline int Graph::eraseVertex(uint _id) {
    if(this->connectivityList.find(_id) == this->connectivityList.end()){
        return errorMassege(UNDEFINED_VERTEX);
    }
    this->V--;
    this->connectivityList.erase(_id);

    for(uint i = 0; i < this->V + 1; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->initByVL();

    return eraseV;
}

inline int Graph::eraseEdge(edge &e) {
    if(this->edgeList.find(e) == this->edgeList.end()){
        return errorMassege(UNDEFINED_EDGE);
    }

    this->edgeList.erase(e);

    this->E--;

    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;

    this->initByEL();

    return eraseE;
}

int Graph::weightGraph(bool isVertex, uint mode) {

}

#endif
