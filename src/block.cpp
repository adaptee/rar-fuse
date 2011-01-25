#include "block.h"

#include <cassert>
#include <fstream>
using std::ifstream ;

#include "flags.h"

Block::Block(std::ifstream & stream, size_t base):
    m_stream(stream),
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


MarkerBlock::MarkerBlock(std::ifstream & stream, size_t base): Block(stream, base)
{

    assert( m_header_crc  == 0x6152) ;
    assert( m_type        == 0x72) ;
    assert( m_flags->hex() == 0x1a21) ;
    assert( m_header_size == 7) ;
    assert( m_added_size  == 0) ;
}


MainBlock::MainBlock(std::ifstream & stream, size_t base): Block(stream, base)
{

}


Flags *
MainBlock::getFlags()
{
    uint16 hex    = readUInt16();
    Flags * flags = new MainFlags(hex);
    return flags;
}


FileBlock::FileBlock(std::ifstream & stream, size_t base): Block(stream, base)
{

}

SubBlock::SubBlock(std::ifstream & stream, size_t base): FileBlock(stream, base)
{

}

EndBlock::EndBlock(std::ifstream & stream, size_t base): Block(stream, base)
{

}



using std::ios;
#include <iostream>
using std::cout;

Block * buildblock(std::ifstream & file, size_t base)
{
    // make sure we always read from the right point
    file.seekg(base, std::ios::beg);

    // read-ahead to fetch type of next block, then move readptr back.
    char buf[4];
    file.read(buf, 3);
    file.seekg (-3, std::ios::cur);

    byte type = buf[2];
    switch(type)
    {
        case 0x72:
            return new MarkerBlock(file, base);
            break;
        case 0x73:
            return new MainBlock(file, base);
            break;
        case 0x74:
            return new FileBlock(file, base);
            break;
        case 0x7a:
            return new SubBlock(file, base);
            break;
        case 0x7b:
            return new EndBlock(file, base);
            break;
        default:
            std::cout<<"[NULL]\n";
            return NULL;
            break;
    };

}

// becasuse we do not have ifstream.length(),
// we have to do a trcik to know its length.
size_t get_leng_of_stream( std::ifstream & stream)
{
    size_t old_pos = stream.tellg();

    stream.seekg(0, std::ios::end);
    size_t length = stream.tellg();
    // do not forget to restore the old position.
    stream.seekg(old_pos, std::ios::beg);

    return length;
}

int main(int argc, char const* argv[])
{
    size_t base = 0;
    std::ifstream file(argv[1], std::ios::in|std::ios::binary);

    size_t length = get_leng_of_stream(file);

    while( base < length )
    {
        Block * block = buildblock(file, base);
        std::cout<<"got one block!\n";
        std::cout<<block->totalSize()<<" bytes\n";
        std::cout<<block->debugRepr();
        base += block->totalSize();
        std::cout<<"\n";
    }

    return 0;
}


