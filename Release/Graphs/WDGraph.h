#ifndef WDGRAPH_H
#define WDGRAPH_H

#include "DirGraph.h"

class WDGraph : public DirGraph
{
public:
    WDGraph(uint V = 0, byte** mat = nullptr, double weight = 0.0);
    virtual ~WDGraph();
};

#endif // WDGRAPH_H
