#ifndef WTREE_H
#define WTREE_H

#include"Tree.h"
#include"UDWGraph.h"

class WTree : public Tree, UDWGraph
{
public:
    WTree();
    virtual ~WTree();
};

#endif // WTREE_H
