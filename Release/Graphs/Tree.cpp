#include "Tree.h"
#include "General.h"

Tree::Tree(uint V, byte** mat) : UDirGraph(V, mat) {}

Tree::~Tree()
{

}

void Tree::print(std::fstream &_to) const
{

}

int Tree::getDegree(uint _Vertex) const
{

}

int Tree::getType() const {
    return tree;
}

bool Tree::isConnected(uint _in, uint _out) const
{

}
