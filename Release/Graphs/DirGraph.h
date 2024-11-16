#ifndef DIRGRAPH_H
#define DIRGRAPH_H
#define STATIC_MEMORY 48U   // 2 bytes from this->V + 4 bytes from this->E

#include"Graph.h"

enum {
    OUT, IN, ALL = 1
};

class DirGraph : public Graph
{
protected:
    uint E;
    byte* upConnectivityMat;
    byte* downConnectivityMat;

public:
    DirGraph(uint _V = 0, byte** mat = nullptr);
	virtual ~DirGraph() override;

    //===========	ACCESSORS	===========
    virtual void print(std::fstream& _to) const override;
    virtual int getDegree(uint _Vertex, bool io) const;
    virtual bool isConnected(uint _in, uint _out) const;
    uint getEdges() const;

    virtual void setEdge(uint _in, uint _out);

    // ===========	 MATH	===========
    virtual Graph& operator+(const Graph& _Right) override;
    virtual Graph& operator+(std::stack<uint>& _Right) override;
    virtual Graph& operator-(const Graph& _Right) override;
    virtual Graph& operator-(uint _Vertex) override;
    virtual int operator()(uint _Vertex) const override;
};

#endif
