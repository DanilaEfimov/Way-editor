#ifndef TREE_H
#define TREE_H

#include "UDirGraph.h"

class Tree : public UDirGraph
{
private:
    uint base;  // max child count
    uint root;
public:
    Tree(uint V, byte** mat);
    virtual ~Tree() override;

    uint getRoot() const;
    virtual void print(std::fstream& _to) const override;
    virtual int getDegree(uint _Vertex) const override;
    virtual int getType() const override;
    virtual bool isConnected(uint _in, uint _out) const override;

    virtual Tree& operator+(std::stack<uint>& _Right) override;
    virtual Tree& operator-(uint _Vertex) override;
};

#endif // TREE_H
