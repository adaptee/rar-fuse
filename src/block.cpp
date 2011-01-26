#include "block.h"

#include <fstream>
#include <sstream>

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
    std::stringstream stream ;

    stream<<"===============================================================\n";

    stream<<"[base]\t\t "<<"0x"<<std::hex<<(uint32)m_base<<"\n";
    stream<<"[type]\t\t "<<"0x"<<std::hex<<(uint16)m_type<<"\n";
    stream<<"[flag]\t\t "<<"0x"<<std::hex<<(uint16)m_flags<<"\n";
    stream<<"[header_size]\t "<<"0x"<<std::hex<<(uint16)m_header_size<<"\n";
    stream<<"[total_size]\t "<<"0x"<<std::hex<<(uint32)totalSize()<<"\n";
    stream<<"\n";

    stream<< extraDebugRepr();

    stream<<"===============================================================\n";
    stream<<"\n";

    return stream.str();
}


string
Block::extraDebugRepr()
{
    return "";
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
Block::readBytes(byte * outbuf, size_t count )
{
    m_stream.read( reinterpret_cast<char*>(outbuf), count);
}


