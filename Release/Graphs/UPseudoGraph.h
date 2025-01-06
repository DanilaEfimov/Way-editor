#ifndef UPSEUDOGRAPH_H
#define UPSEUDOGRAPH_H

#include "UDirGraph.h"

class UPseudoGraph : public UDirGraph
{
protected:
    byte* loops;
public:
    UPseudoGraph(uint V, byte** mat);
    virtual ~UPseudoGraph();

    virtual int getType() const override;
    virtual void print(std::fstream& _to) const override;
    virtual std::string show() const override;
    virtual int getDegree(uint _Vertex) const override;
    virtual bool isConnected(uint _in, uint _out) const override;

    virtual UDirGraph& operator-(uint _Vertex) override;
};

#endif // UPSEUDOGRAPH_H
