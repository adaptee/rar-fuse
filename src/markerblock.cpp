#include "assert.h"
#include "markerblock.h"


MarkerBlock::MarkerBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    assert( m_header_crc  == 0x6152) ;
    assert( m_type        == 0x72) ;
    assert( m_flags       == 0x1a21) ;
    assert( m_header_size == 7) ;
    assert( m_added_size  == 0) ;
}

