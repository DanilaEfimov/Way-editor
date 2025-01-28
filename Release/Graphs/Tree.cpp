#include "Tree.h"
#include "General.h"

Tree::Tree(uint V, byte** mat) : UDirGraph(V, mat) {}

Tree::~Tree()
{

}

uint Tree::getRoot() const {
    return this->root;
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

Tree &Tree::operator+(std::stack<uint> &_Right)
{

}

Tree &Tree::operator-(uint _Vertex)
{

}
