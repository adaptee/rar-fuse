#include "block.h"

#include <cassert>
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


MarkerBlock::MarkerBlock(std::ifstream & stream, size_t base): Block(stream, base)
{

    assert( m_header_crc  == 0x6152) ;
    assert( m_type        == 0x72) ;
    assert( m_flags == 0x1a21) ;
    assert( m_header_size == 7) ;
    assert( m_added_size  == 0) ;
}


MainBlock::MainBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    assert( m_type == 0x73) ;
}


bool
MainBlock::isVolume()
{
    return m_flags & 0x0001;
}

bool
MainBlock::hasEmbededComment()
{
    return m_flags & 0x0002;
}

    bool
MainBlock::isLocked()
{
    return m_flags & 0x0004;
}

    bool
MainBlock::isSolid()
{
    return m_flags & 0x0008;
}

//scheme xxxx.partN.rar
bool
MainBlock::useNewSchemeForVolumeName()
{
    return m_flags & 0x0010;
}


bool
MainBlock::hasAuthInfo()
{
    return m_flags & 0x0020;
}

bool
MainBlock::hasRecoveryRecord()
{
    return m_flags & 0x0040;
}

bool
MainBlock::hasPassword()
{
    return m_flags & 0x0080;
}

bool
MainBlock::isFirstVolume()
{
    return m_flags & 0x0100;
}

bool
MainBlock::encryptVersion()
{
    return m_flags & 0x0200;
}



FileBlock::FileBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    parse();

}


void
FileBlock::parse()
{
    m_low_pack_size    = m_added_size;
    m_low_unpack_size  = readUInt32();
    m_host_os          = readByte();
    m_filecrc          = readUInt32();
    m_filetime         = readUInt32();
    m_unpack_version   = readByte();
    m_pack_method      = readByte();
    m_filename_size    = readUInt16();
    m_fileattr         = readUInt32();

    //m_high_pack_size   = getHighPackSize();
    //m_high_unpack_size = getHighUnPackSize();
    //m_filename         = getFileName();
    //m_salt             = getSalt();
    //m_ext_filetime     = getExtFileTime();

}


bool
FileBlock::isMissingPart()
{
    return m_flags & 0x0001;
}

bool
FileBlock::needMissingPart()
{
    return m_flags & 0x0002;
}

bool
FileBlock::hasPassword()
{
    return m_flags & 0x0004;
}

bool
FileBlock::hasEmbededComment()
{
    return m_flags & 0x0008;
}

bool
FileBlock::isSolid()
{
    return m_flags & 0x0010;
}

bool
FileBlock::sizeOfDictInKB()
{
    //FIXME
    //mask = 0x00e0
    //result = m_flags & mask
    return 64;
}

bool
FileBlock::isDir()
{
    return m_flags & 0x00e0 == 0x00e0;
}

bool
FileBlock::isLargeFile()
{
    return m_flags & 0x0100;
}

bool
FileBlock::useUnicode()
{
    return m_flags & 0x0200;
}

bool
FileBlock::hasSalt()
{
    return m_flags & 0x0400;
}

bool
FileBlock::hasExtTime()
{
    return m_flags & 0x1000;
}

bool
FileBlock::hasExtFlags()
{
    return m_flags & 0x2000;
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
            return NULL;
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


