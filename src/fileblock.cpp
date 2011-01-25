#include "fileblock.h"

FileBlock::FileBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    parse();

}


void
FileBlock::parse()
{
    // for FileBlock, (m_flags & 0x8000)  is always true, which
    // implies that optional filed `m_added_size` is alwasy present.
    // so this field is intrepreted as low_pack_size
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
