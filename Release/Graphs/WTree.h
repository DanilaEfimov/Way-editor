#ifndef WTREE_H
#define WTREE_H

#include"Tree.h"

class WTree : public Tree
{
public:
    WTree(uint V = 0, byte** mat = nullptr, double weight = 0.0);
    virtual ~WTree();
};

#endif // WTREE_H
