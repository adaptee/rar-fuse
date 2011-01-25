#include "block.h"

#include <fstream>
using std::ifstream ;

#include "flags.h"

Block::Block(std::ifstream & in, size_t base):
    m_stream(in),
    m_base(base)
{
    parse();
}

Block::~Block()
{

}

void Block::parse()
{
    m_header_crc   = readUInt16();
    m_type  = readByte();
    m_flags = getFlags();
    m_header_size  = readUInt16();
    m_added_size   = getAddedSize();

}

uint32
Block::getAddedSize()
{
    if ( m_flags->hasAddedSize() )
    {
        return readUInt32();
    }
    else
    {
        return 0;
    }
};

Flags *
Block::getFlags()
{
    uint16 hex    = readUInt16();
    Flags * flags = new Flags(hex);
    return flags;
}


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

#include <iostream>
using std::cout;
int main(int argc, char const* argv[])
{
    std::ifstream file(argv[1]);

    Block b(file, 0);
    std::cout<<b.debugRepr();

    return 0;
}


