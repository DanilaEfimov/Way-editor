#ifndef DPSEUDOGRAPH_H
#define DPSEUDOGRAPH_H

#include "DirGraph.h"

class DPseudoGraph : public DirGraph
{
public:
    DPseudoGraph(uint V, byte** mat);
    virtual ~DPseudoGraph();
};

#endif // DPSEUDOGRAPH_H
