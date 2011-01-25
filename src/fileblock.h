#ifndef FILEBLOCK_GUARD
#define FILEBLOCK_GUARD

#include "block.h"

class FileBlock: public Block
{
public:
    FileBlock(std::ifstream & stream, size_t base);

    uint64 packSize();
    uint64 unpackSize();

protected:
    virtual string extraDebugRepr();

    // predicators based upon flags
    bool isMissingPart();
    bool needMissingPart();
    bool hasPassword();
    bool hasEmbededComment();
    bool isSolid();
    bool sizeOfDictInKB();
    bool isDir();
    bool isLargeFile();
    bool useUnicode();
    bool hasSalt();
    bool hasExtTime();
    bool hasExtFlags();

    // basic data
    uint32  m_low_pack_size;
    uint32  m_low_unpack_size;
    byte    m_host_os;
    uint32  m_filecrc;
    uint32  m_filetime;
    byte    m_unpack_version;
    byte    m_pack_method;
    uint16  m_filename_size;
    uint32  m_fileattr;

    uint32 m_high_pack_size ;
    uint32 m_high_unpack_size ;

    byte m_salt[8] ;

    wchar_t m_filename[512];

private:
    void parse();
    uint32 getOptionalHigherSize();

    void getSalt(byte * buf);
    void getFileName();
    void getExtFileTime();

};


#endif /* end of include guard: FILEBLOCK_GUARD */
