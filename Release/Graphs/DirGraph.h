#ifndef DIRGRAPH_H
#define DIRGRAPH_H
#define STATIC_MEMORY 48   // 2 bytes from this->V + 4 bytes from this->E

#include"Graph.h"
#include<stack>

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
    virtual void eraseEdge(uint _in, uint _out);

    // ===========	 MATH	===========
    std::stack<uint>& BFS(uint _root = 1) const;
    std::stack<uint>& DFS(uint _root = 1) const;
    std::stack<uint>& EulerCycle(uint _begin = 1) const;
    DirGraph& operator+=(const DirGraph& _Right);
    DirGraph& operator+(std::stack<uint>& _Right);
    DirGraph& operator-=(const DirGraph& _Right);
    virtual DirGraph& operator-(uint _Vertex);
    virtual int operator()(uint _Vertex) const override;
};

#endif

/***************************************************************
* DirGraph performed like two bit vectors for up-half (upper main diagonal)
* and down-half (lowwer main diagonal) parts of connectivity matrix
* i.e. for V vertex we have V * (V - 1) BITS instead V * V BYTES
* it cuts memory amount for > 8 times.
* e.g. full 3-vertex graph connectivity matrix:
*
*						x|	1	|	2	|	3
*						-+------+-------+------
*						1|	0	|	1	|	1
*						-+------+-------+------
*						2|	1	|	0	|	1
*						-+------+-------+------
*						3|	1	|	1	|	0
*
* Here we have to get only three bits: [1][2], [1][3], [2][3], [2][1], [3][1], [3][2]
* and these bits written into vectors row by row: "1 1 1 0 0 0 0 0 + 1 1 1 0 0 0 0 0"
* instead 0x00FFFFFF00FFFFFF00 -> 0b1110000011100000    (9 bytes to 2)
****************************************************************/
