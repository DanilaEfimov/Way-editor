#ifndef GRAPH_H
#define GRAPH_H

#include<fstream>
#include<stack>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char byte_t, byte;

class Graph
{
protected:
    ushort V;                                                   // short is 2 bytes, but edges is O(V^2) ~4 bytes
public:
	Graph(uint _V = 0);											// default constructor
	virtual ~Graph();

	//===========	ACCESSORS	===========
	virtual void print(std::fstream& _to) const = 0;

	// ===========	 MATH	===========
    virtual Graph& operator+(const Graph& _Right) = 0;			// union of graphs
    virtual Graph& operator+(std::stack<uint>& _Right) = 0;		// adding new vertex
    virtual Graph& operator-(const Graph& _Right) = 0;			// difference of graphs
	virtual Graph& operator-(uint _Vertex) = 0;					// deleting vertex
    virtual int operator()(uint _Vertex) const = 0;				// degree of _Vertex (-1 for undefined _V)
};

#endif

/***************************************************************
**	WarerCode (c) 2024. All rights reserved.
**  Abstract class of math graph. Graph's interface.
**	Child tree:
**	Graph --+
**			|
**			+ UDirGraph ----+
**			|				|
**			|				+ UDWGraph	(weighted graph)
**			|				|
**			|				+ UPseudoGraph
**			|
**			+ DirGraph -----+
**							|
**							+ WDGraph
**							|
**							+ PseudoGraph
****************************************************************/
