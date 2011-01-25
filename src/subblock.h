#ifndef SUBBLOCK_GUARD
#define SUBBLOCK_GUARD

#include "fileblock.h"

class SubBlock: public FileBlock
{
public:
    SubBlock(std::ifstream & stream, size_t base);
};


#endif /* end of include guard: SUBBLOCK_GUARD */
