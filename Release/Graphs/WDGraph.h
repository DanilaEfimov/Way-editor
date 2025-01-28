#ifndef WDGRAPH_H
#define WDGRAPH_H
#define WORD sizeof(double) // in GCC x64 means 8 bytes

#include "DirGraph.h"

typedef double word, word_t;

class WDGraph : public DirGraph
{
protected:
    word* vWeights; // + 64*V bits
    word* eWeights; // + O(V^2) bits
public:
    WDGraph(uint V = 0, byte** mat = nullptr, double weight = 0.0);
    virtual ~WDGraph();

    //===========	ACCESSORS	===========
    virtual void print(std::fstream& _to) const override;
    virtual int  getType() const override;
    virtual void setNormalVWeights();
    virtual void setMedianVWeights();
    virtual void setNormalEWeights();
    virtual void setMedianEWeights();
    double getWeightV(uint _Vertex) const;
    double getWeightE(uint _in, uint _out) const;
    void setVWeight(uint _Vertex, double _value = 0.0);
    void setEWeight(uint _in, uint _out, double _value = 0.0);
    void setRandomWeights(uint _seedV = 0, uint _seedE = 0,
                          double _begin = 0.0, double _end = 100.0);
    double getMaxEweight() const;
    double getMaxVWeight() const;

    virtual void setEdge(uint _in, uint _out) override;
    virtual void eraseEdge(uint _in, uint _out) override;

    // ===========	 MATH	===========
    std::stack<uint>& Dejcstra(uint _in, uint _out) const;
    virtual WDGraph& operator-(uint _Vertex) override;
    virtual WDGraph& operator+(std::stack<uint>& _Right) override;
};

#endif // WDGRAPH_H
