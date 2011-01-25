#ifndef BLOCK_GUARD
#define BLOCK_GUARD

#include <iosfwd>
#include <string>
using std::string ;

#include "defs.h"

class Block
{

public:
    Block(std::ifstream & stream, size_t base);

    size_t totalSize();
    string debugRepr();

protected:

    // predicators based upon flags
    bool shouldSkipUnknownBlock();
    bool hasAddedSize();
    virtual string extraDebugRepr();

    // basic data
    uint16  m_header_crc;
    byte    m_type;
    uint16  m_flags;
    uint16  m_header_size;
    uint32  m_added_size;

    // data retriver
    byte   readByte();
    uint16 readUInt16();
    uint32 readUInt32();
    void   readBytes( byte * outbuf, size_t count );

private:
    void parse();
    uint32 getAddedSize();

    std::ifstream & m_stream;
    const size_t    m_base;
};


#endif /* end of include guard: BLOCK_GUARD */
