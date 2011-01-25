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

    virtual size_t totalSize();
    virtual string debugRepr();


protected:

    byte   readByte();
    uint16 readUInt16();
    uint32 readUInt32();
    void   readBytes(size_t count, char * outbuf, size_t bufsize );

    // flags realted predicators.
    bool shouldSkipUnknownBlock();
    bool hasAddedSize();

    uint16  m_header_crc;
    byte    m_type;
    uint16  m_flags;
    uint16  m_header_size;
    uint32  m_added_size;

private:
    void parse();

    uint32 getAddedSize();


    std::ifstream & m_stream;
    const size_t m_base;
};


class MarkerBlock: public Block
{
public:
    MarkerBlock(std::ifstream & stream, size_t base);
};

class MainBlock: public Block
{
public:
    MainBlock(std::ifstream & stream, size_t base);

protected:

    bool isVolume();
    bool hasEmbededComment();
    bool isLocked();
    bool isSolid();
    bool useNewSchemeForVolumeName();
    bool hasAuthInfo();
    bool hasRecoveryRecord();
    bool hasPassword();
    bool isFirstVolume();
    bool encryptVersion();;

};


class FileBlock: public Block
{
public:
    FileBlock(std::ifstream & stream, size_t base);

protected:

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

    uint32  m_low_pack_size;
    uint32  m_low_unpack_size;
    byte    m_host_os;
    uint32  m_filecrc;
    uint32  m_filetime;
    byte    m_unpack_version;
    byte    m_pack_method;
    uint16  m_filename_size;
    uint32  m_fileattr;

protected:
    void parse();

};



class SubBlock: public FileBlock
{
public:
    SubBlock(std::ifstream & stream, size_t base);
};

class EndBlock: public Block
{
public:
    EndBlock(std::ifstream & stream, size_t base);

};


#endif /* end of include guard: BLOCK_GUARD */
