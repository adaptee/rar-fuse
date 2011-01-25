#ifndef BLOCK_GUARD
#define BLOCK_GUARD

#include <iosfwd>
#include <string>
using std::string ;

#include "defs.h"

class Flags;

class Block
{

public:
    Block(std::ifstream & in, size_t base);
    ~Block();

    virtual size_t totalSize();
    virtual string debugRepr();

protected:
    uint16 m_header_crc;
    byte   m_type;
    Flags * m_flags;
    uint16 m_header_size;
    uint32 m_added_size;

private:
    void parse();

    Flags * getFlags();
    uint32 getAddedSize();

    byte readByte();
    uint16 readUInt16();
    uint32 readUInt32();
    void readBytes(size_t count, char * outbuf, size_t bufsize );

    std::ifstream & m_stream;
    const size_t m_base;
};


class MarkBlock: public Block
{

};

class MainBlock: public Block
{

};


class FileBlock: public Block
{

};

class SubBlock: public FileBlock
{

};

class EndBlock: Block
{

};


#endif /* end of include guard: BLOCK_GUARD */
