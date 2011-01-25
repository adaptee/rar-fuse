#include "flags.h"

Flags::Flags(uint16 hex): m_hex(hex)
{

}

bool
Flags::shouldSkipUnknownBlock()
{
    return m_hex & 0x4000 ;
}

bool
Flags::hasAddedSize()
{
    return m_hex & 0x8000;
}
