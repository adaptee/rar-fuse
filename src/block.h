#ifndef BLOCK_GUARD
#define BLOCK_GUARD

#include <iosfwd>
#include <string>
using std::wstring ;

#include "defs.h"

class Block
{

public:
    Block(std::ifstream & stream, size_t base);

    size_t totalSize() const;
    wstring debugRepr() const;

protected:
    virtual wstring extraDebugRepr() const;

    // predicators based upon flags
    bool shouldSkipUnknownBlock() const;
    bool hasAddedSize() const ;

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

    std::ifstream & m_stream;
    const size_t    m_base;

private:
    void parse();
    uint32 getAddedSize();

};


#endif /* end of include guard: BLOCK_GUARD */
