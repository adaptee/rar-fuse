#ifndef ENDBLOCK_GUARD
#define ENDBLOCK_GUARD

#include "block.h"

class EndBlock: public Block
{
public:
    EndBlock(std::ifstream & stream, size_t base);

};


#endif /* end of include guard: ENDBLOCK_GUARD */


