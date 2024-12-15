#ifndef TREE_H
#define TREE_H

#include "UDirGraph.h"

class Tree : public UDirGraph
{
private:
    uint base;  // max child count
    uint root;
public:
    Tree();
    virtual ~Tree() override;

    virtual void print(std::fstream& _to) const override;
    virtual int getDegree(uint _Vertex) const override;
    virtual bool isConnected(uint _in, uint _out) const;
};

#endif // TREE_H
