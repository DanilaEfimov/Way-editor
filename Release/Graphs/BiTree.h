#ifndef BITREE_H
#define BITREE_H

#include "Tree.h"

typedef struct Node {
    Node* left;
    Node* right;
} node, node_t;

class BiTree : public Tree
{
public:
    BiTree(uint V = 0, byte** mat = nullptr);
    virtual ~BiTree();
};

#endif // BITREE_H
