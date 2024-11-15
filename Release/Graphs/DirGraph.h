#ifndef UDIRGRAPH_H
#define UDIRGRAPH_H

#include"Graph.h"

class DirGraph : public Graph
{
protected:
	uint E;
	byte* connectivityVector;

public:
    DirGraph(uint _V = 0, byte** mat = nullptr);
	virtual ~DirGraph() override;

    //===========	ACCESSORS	===========
    virtual void print(std::fstream& _to) const override;
    virtual int getDegree(uint _Vertex) const;
    virtual bool isConnected(uint _in, uint _out) const;
    uint getEdges() const;

    virtual void setEdge(uint _in, uint _out);

    // ===========	 MATH	===========
    virtual Graph& operator+(Graph& _Right) override;
    virtual Graph& operator+(std::stack<uint>& _Right) override;
    virtual Graph& operator-(Graph& _Right) override;
    virtual Graph& operator-(uint _Vertex) override;
    virtual int operator()(uint _Vertex) override;
};

#endif
