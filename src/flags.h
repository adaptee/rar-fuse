#ifndef FLAGS_GUARD
#define FLAGS_GUARD
#include "defs.h"

class Flags
{
public:
    Flags(uint16 hex);

    bool shouldSkipUnknownBlock();
    bool hasAddedSize();

protected:
    uint16 m_hex;

};



#endif /* end of include guard: FLAGS_GUARD */
