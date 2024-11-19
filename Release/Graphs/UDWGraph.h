#ifndef UDWGRAPH_H
#define UDWGRAPH_H
#define WORD sizeof(double) // GCC x64 8 bytes

#include "UDirGraph.h"

typedef double word, word_t;

class UDWGraph : public UDirGraph
{
protected:
    word* vWeights; // + 64*V bits
    word* eWeights; // + O(V*V) bits
public:
    UDWGraph(uint _V = 0, byte** mat = nullptr, double weights = 0.0f);
    virtual ~UDWGraph() override;

    //===========	ACCESSORS	===========
    virtual void print(std::fstream& _to) const override;
    virtual int getDegree(uint _Vertex) const override;
    virtual bool isConnected(uint _in, uint _out) const override;
    virtual void setEdge(uint _in, uint _out) override;
    virtual void setNormalVWeights();
    virtual void setMedianVWeights();
    virtual void setNormalEWeights();
    virtual void setMedianEWeights();
    double getWeightV(uint _Vertex) const;
    double getWeightE(uint _in, uint _out) const;
    void setVWeight(uint _Vertex, double _value);
    void setRandomWeights(uint _seedV = 0, uint _seedE = 0,
                          double _begin = 0.0, double _end = 100.0);

    // ===========	 MATH	===========
    virtual std::stack<uint>& Dejcstra(uint _in, uint _out) const;
    virtual Graph& operator+(const Graph& _Right) override;
    virtual Graph& operator+(std::stack<uint>& _Right) override;
    virtual Graph& operator-(const Graph& _Right) override;
    virtual Graph& operator-(uint _Vertex) override;
    virtual int operator()(uint _Vertex) const override;
};

#endif // UDWGRAPH_H
