#ifndef FILEBLOCK_GUARD
#define FILEBLOCK_GUARD

#include "block.h"
#include <fstream>

class FileBlock: public Block
{
public:
    FileBlock(std::ifstream & stream, size_t base);
    ~FileBlock();

    wstring filename() const;

    bool isDir() const;
    bool isCompressed() const;

    uint64 packSize() const;
    uint64 unpackSize() const;

    uint32 time() const;

    size_t read( void * dest, size_t offset, size_t count);

    const byte * data() ;

protected:
    virtual wstring extraDebugRepr() const;

    // predicators based upon flags
    bool isMissingPart() const ;
    bool needMissingPart() const ;
    bool hasPassword() const ;
    bool hasEmbededComment() const ;
    bool isSolid() const ;
    bool sizeOfDictInKB() const ;
    bool isLargeFile() const ;
    bool useUnicode() const ;
    bool hasSalt() const ;
    bool hasExtTime() const ;
    bool hasExtFlags() const ;

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

    // make copy-constructor and copy-assignment un-usable
    FileBlock( const FileBlock & block);
    void operator =(const FileBlock & block);

    void parse();
    uint32 getOptionalHigherSize();

    void getSalt(byte * buf);
    void getFileName();
    void getExtFileTime();

    void getData();

    byte * m_data;
};


#endif /* end of include guard: FILEBLOCK_GUARD */
