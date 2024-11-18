#ifndef UDWGRAPH_H
#define UDWGRAPH_H
#define WORD sizeof(double) // GCC x64 8 bytes

#include "UDirGraph.h"

typedef double word, word_t;

class UDWGraph : public UDirGraph
{
protected:
    word* weights; // + 64*V bits
public:
    UDWGraph(uint _V = 0, byte** mat = nullptr, double weights = 0.0f);
    virtual ~UDWGraph() override;

    //===========	ACCESSORS	===========
    virtual void print(std::fstream& _to) const override;
    virtual int getDegree(uint _Vertex) const override;
    virtual bool isConnected(uint _in, uint _out) const override;
    virtual void setEdge(uint _in, uint _out) override;
    virtual void setNormalWeights();
    virtual void setMedianWeights();
    void setWeight(uint _Vertex, double _value);
    void setRandomWeights(uint _seed1 = 0, uint _seed2 = 0, double _begin = 0.0, double _end = 100.0);

    // ===========	 MATH	===========
    virtual std::stack<uint>& Dejcstra(uint _in, uint _out) const;
    virtual Graph& operator+(const Graph& _Right) override;
    virtual Graph& operator+(std::stack<uint>& _Right) override;
    virtual Graph& operator-(const Graph& _Right) override;
    virtual Graph& operator-(uint _Vertex) override;
    virtual int operator()(uint _Vertex) const override;
};

#endif // UDWGRAPH_H
