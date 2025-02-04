#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char byte_t, byte;

class Graph
{
protected:
    ushort V;        // short is 2 bytes, but edges is O(V^2) ~4 bytes
public:
	Graph(uint _V = 0);											// default constructor
	virtual ~Graph();

    bool empty() const { return this->V == 0; };

	//===========	ACCESSORS	===========
    virtual void print(std::fstream&) const = 0;
    virtual std::string& show() const = 0;
    virtual bool isConnected(uint _in, uint _out) const = 0;
    virtual int getType() const = 0;
    ushort getV() const {return this->V;};

    // ===========	 MATH	===========
    virtual int operator()(uint) const = 0;				// degree of _Vertex (-1 for undefined _V)
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
**			|				+ UPseudoGraph
**			|				|
**			|				+ UDWGraph
**			|               |
**			|               + Tree ---------+ WTree
**			|                           |
**			|                           +---+ BiTree
**			|
**			+ DirGraph -----+
**							|
**							+ WDGraph
**							|
**							+ DPseudoGraph
****************************************************************/
