#ifndef MARKERBLOCK_GUARD
#define MARKERBLOCK_GUARD

#include "block.h"

class MarkerBlock: public Block
{
public:
    MarkerBlock(std::ifstream & stream, size_t base);
};


#endif /* end of include guard: MARKERBLOCK_GUARD */
