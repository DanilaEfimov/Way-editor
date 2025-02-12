#ifndef WTREE_H
#define WTREE_H

#include"Tree.h"

class WTree : public Tree
{
public:
    WTree(uint V = 0, byte** mat = nullptr, double weight = 0.0);
    virtual ~WTree();

    void setVWeight(uint _Vertex, double value);
    double getWeightV(uint _Vertex);
};

#endif // WTREE_H
