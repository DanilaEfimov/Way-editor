#ifndef UDIRGRAPH_H
#define UDIRGRAPH_H
#define STATIC_MEMORY 48U   // 2 bytes from this->V + 4 bytes from this->E

#include"Graph.h"

class UDirGraph : public Graph								// most primitive graph class
{
protected:
    uint E;
    byte_t* connectivityVector;                             // byte-vector of connectivity matrix

public:
    UDirGraph(uint _V = 0, byte** mat = nullptr);
	virtual ~UDirGraph() override;

	//===========	ACCESSORS	===========
	virtual void print(std::fstream& _to) const override;
	virtual int getDegree(uint _Vertex) const;
    virtual bool isConnected(uint _in, uint _out) const;
    uint getEdges() const;

	virtual void setEdge(uint _in, uint _out);

	// ===========	 MATH	===========
    std::stack<uint>& BFS(uint _root = 1) const;
    std::stack<uint>& DFS(uint _root = 1) const;
    std::stack<uint>& EulerCycle(uint _begin = 1) const;
    virtual Graph& operator+(const Graph& _Right) override;
	virtual Graph& operator+(std::stack<uint>& _Right) override;
    virtual Graph& operator-(const Graph& _Right) override;
	virtual Graph& operator-(uint _Vertex) override;
    virtual int operator()(uint _Vertex) const override;
};

/***************************************************************
* UDirGraph performed like bit vector of up-half (upper main diagonal)
* i.e. for V vertex we have V * (V - 1) / 2 BITS instead V * V BYTES
* it cuts memory amount for 16 times.
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
* Here we have to get only three bits: [1][2], [1][3], [2][3]
* and these bits written into vector row by row: "1 1 1 0 0 0 0 0"
* instead 0x00FFFFFF00FFFFFF00 -> 0b11100000	(9 bytes to 1)
****************************************************************/

#endif 
