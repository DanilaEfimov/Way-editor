#ifndef UDWGRAPH_H
#define UDWGRAPH_H
#define WORD sizeof(double) // in GCC x64 means 8 bytes

#include "UDirGraph.h"

typedef double word, word_t;

class UDWGraph : public UDirGraph
{
protected:
    word* vWeights; // + 64*V bits
    word* eWeights; // + O(V^2) bits
public:
    UDWGraph(uint _V = 0, byte** mat = nullptr, double weights = 0.0f);
    virtual ~UDWGraph() override;

    //===========	ACCESSORS	===========
    virtual void print(std::fstream& _to) const override;
    virtual void setEdge(uint _in, uint _out) override;
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
    virtual void eraseEdge(uint _in, uint _out) override;

    // ===========	 MATH	===========
    std::stack<uint>& Dejcstra(uint _in, uint _out) const;
    virtual UDirGraph& operator-(uint _Vertex) override;
    int operator()(uint _Vertex) const override;
};

#endif // UDWGRAPH_H

// some about weights:
/*
*   eWeights:   [1,2],[1,3],[1,4],[1,5]..,[2,3],[2,4],[2,5]..,[3,4],[3,5],[...]
*   so weight of edge between i and j vertex placed at (i * V + j) place in
*   bit vector
*/
