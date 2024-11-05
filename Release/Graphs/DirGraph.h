#ifndef UDIRGRAPH_H
#define UDIRGRAPH_H

#include"Graph.h"

class DirGraph : public Graph
{
protected:
	uint E;

	byte* connectivityVector;
public:
	DirGraph(uint _V, byte** mat);
	virtual ~DirGraph() override;
};

#endif