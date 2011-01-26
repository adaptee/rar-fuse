#include <sstream>
#include "string.h"
#include "fileblock.h"
#include "filename.h"

FileBlock::FileBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    parse();

}

uint64
FileBlock::packSize()
{
    return m_low_pack_size + (uint64(m_high_pack_size) << 32);
}

uint64
FileBlock::unpackSize()
{
    return m_low_unpack_size + (uint64(m_high_unpack_size) << 32);
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

    m_high_pack_size   = getOptionalHigherSize();
    m_high_unpack_size = getOptionalHigherSize();

    getFileName();
    getSalt(m_salt);
    getExtFileTime();

}

string
FileBlock::extraDebugRepr()
{
    std::stringstream stream;
    stream<<"[isDir]\t\t "<<isDir()<<"\n";
    stream<<"[HostOS]\t "<<(uint16)m_host_os<<"\n";
    stream<<"[useUnicode]\t "<<useUnicode()<<"\n";
    stream<<"[hasSalt]\t "<<hasSalt()<<"\n";
    stream<<"[hasPassword]\t "<<hasPassword()<<"\n";
    stream<<"[hasExtTime]\t "<<hasExtTime()<<"\n";
    stream<<"\n";
    stream<<"[filename]\n";
    for(int i=0; i< wcslen(m_filename) ; i++)
        stream<<std::hex<<(long long)m_filename[i]<<"\n";

    return stream.str();

}

void
FileBlock::getFileName()
{
    // FIXME: hard-coded, maybe too much or not enough.
    byte buffer[1024] = { 0x0, };
    readBytes(buffer, m_filename_size);
    const byte * natives   = buffer;

    if ( useUnicode())
    {
        const byte * delimiter = (const byte *)strchr( (const char *)buffer, 0x0);
        const byte * weirds    = delimiter + 1;
        size_t size_of_weirds  = m_filename_size - ( weirds - natives);

        convert_weird_to_unicode((const char *)natives,
                                 weirds,
                                 size_of_weirds,
                                 m_filename,
                                 sizeof(m_filename)/sizeof(m_filename[0])
                                 );
    }
    else
    {
        for(int i=0; i < m_filename_size ; i++)
            m_filename[i] = (wchar_t) natives[i];
        m_filename[m_filename_size] = 0;
    }

}

uint32
FileBlock::getOptionalHigherSize()
{
    if (isLargeFile())
        return readUInt32();
    else
        return 0;
}

void
FileBlock::getSalt(byte * buf)
{
    if ( hasSalt() )
        readBytes(buf, 8);
}

void
FileBlock::getExtFileTime()
{

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
