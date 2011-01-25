#include "assert.h"
#include "block.h"

#include <fstream>
using std::ifstream ;

Block::Block(std::ifstream & stream, size_t base):
    m_stream(stream),
    m_base(base)
{
    parse();
}

void Block::parse()
{
    m_header_crc  = readUInt16();
    m_type        = readByte();
    m_flags       = readUInt16();
    m_header_size = readUInt16();
    m_added_size  = getAddedSize();
}

uint32
Block::getAddedSize()
{
    if ( hasAddedSize() )
        return readUInt32();
    else
        return 0;
};


string
Block::debugRepr()
{
    return string("hello\n");
}

size_t
Block::totalSize()
{
    return m_header_size + m_added_size ;
}


bool
Block::shouldSkipUnknownBlock()
{
    return m_flags & 0x4000 ;
}

bool
Block::hasAddedSize()
{
    return m_flags & 0x8000;
}


byte
Block::readByte()
{
    return (byte) m_stream.get();
}

uint16
Block::readUInt16()
{
    byte b0  = readByte();
    byte b1 = readByte();

    return b0 + (b1 << 8) ;

}

uint32
Block::readUInt32()
{
    byte b0 = readByte();
    byte b1 = readByte();
    byte b2 = readByte();
    byte b3 = readByte();

    return b0 + (b1 << 8) + (b2 << 16) + (b3 << 24) ;
}

void
Block::readBytes(size_t count, char * outbuf, size_t bufsize )
{
    assert( bufsize >= count);
    m_stream.read(outbuf, count);
}


