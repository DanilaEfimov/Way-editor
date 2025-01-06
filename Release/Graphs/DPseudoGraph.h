#ifndef DPSEUDOGRAPH_H
#define DPSEUDOGRAPH_H

#include "DirGraph.h"

class DPseudoGraph : public DirGraph
{
protected:
    byte* loops;
public:
    DPseudoGraph(uint V, byte** mat);
    virtual ~DPseudoGraph();

    virtual int getType() const override;
    virtual void print(std::fstream& _to) const override;
    virtual std::string show() const override;
    virtual int getDegree(uint _Vertex, bool io) const override;
    virtual bool isConnected(uint _in, uint _out) const override;

    virtual DirGraph& operator-(uint _Vertex) override;
};

#endif // DPSEUDOGRAPH_H
