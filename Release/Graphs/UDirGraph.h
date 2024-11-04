#ifndef UDIRGRAPH_H
#define UDIRGRAPH_H

#include"Graph.h"

typedef unsigned char byte_t, byte;

class UDirGraph : public Graph								// most primitive graph class
{
protected:
	uint E;

	byte_t* connectivityVector;								// byte-vector of connectivity matrix
public:
	UDirGraph(uint _V, byte** mat);
	virtual ~UDirGraph() override;

	//===========	ACCESSORS	===========
	virtual void print(std::fstream& _to) const override;
	virtual int getDegree(uint _Vertex) const;

	// ===========	 MATH	===========
	virtual Graph& operator+(Graph& _Right) override;
	virtual Graph& operator+(std::stack<uint>& _Right) override;
	virtual Graph& operator-(Graph& _Right) override;
	virtual Graph& operator-(uint _Vertex) override;
	virtual int operator()(uint _Vertex) override;
};

/***************************************************************
* UDirGraph performed like bit vector of up-half (upper main diagonal)
* i.e. for V vertex we have (V * V + 1) / 2 BITS instead V * V BYTES
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
* and these bits written into vector string by string: "1 1 1 0 0 0 0 0"
* instead 0x00FFFFFF00FFFFFF00 -> 0b111	(9 bytes to 1)
****************************************************************/

#endif 