#ifndef UPSEUDOGRAPH_H
#define UPSEUDOGRAPH_H

#include "UDirGraph.h"

class UPseudoGraph : public UDirGraph
{
public:
    UPseudoGraph(uint V, byte** mat);
    virtual ~UPseudoGraph();
};

#endif // UPSEUDOGRAPH_H
