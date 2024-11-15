#ifndef GRAPH_H
#define GRAPH_H

#include<fstream>
#include<stack>

typedef unsigned int uint;
typedef unsigned char byte_t, byte;

class Graph
{
protected:
	uint V;
public:
	Graph(uint _V = 0);											// default constructor
	virtual ~Graph();

	//===========	ACCESSORS	===========
	virtual void print(std::fstream& _to) const = 0;

	// ===========	 MATH	===========
	virtual Graph& operator+(Graph& _Right) = 0;				// union of graphs
	virtual Graph& operator+(std::stack<uint>& _Right) = 0;		// adding new vertex
	virtual Graph& operator-(Graph& _Right) = 0;				// difference of graphs
	virtual Graph& operator-(uint _Vertex) = 0;					// deleting vertex
	virtual int operator()(uint _Vertex) = 0;					// degree of _Vertex (-1 for undefined _V)
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
**			|				+ WGraph	(weighted graph)
**			|				|
**			|				+ UPseudoGraph
**			|
**			+ DirGraph -----+
**							|
**							+ WDGraph
**							|
**							+ PseudoGraph
****************************************************************/
