#include "fileblock.h"

#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "filename.h"

FileBlock::FileBlock(std::ifstream & stream, size_t base):
Block(stream, base),
m_data(NULL)
{
    parse();
}
FileBlock::~FileBlock()
{
    delete[] m_data;
}

wstring
FileBlock::filename() const
{
    // prepend the missing '/', to make it more consistent
    return wstring(L"/" + wstring(m_filename) );
}


uint64
FileBlock::packSize() const
{
    return m_low_pack_size + (uint64(m_high_pack_size) << 32);
}

uint64
FileBlock::unpackSize() const
{
    return m_low_unpack_size + (uint64(m_high_unpack_size) << 32);
}

uint32
FileBlock::time() const
{
    return m_filetime;
}

uint32
FileBlock::atime() const
{
    if( hasExtTime() )
        return m_atime;
    else
        return m_filetime;
}

uint32
FileBlock::ctime() const
{
    if( hasExtTime() )
        return m_ctime;
    else
        return m_filetime;
}

uint32
FileBlock::mtime() const
{
    if( hasExtTime() )
        return m_mtime;
    else
        return m_filetime;
}


bool FileBlock::isCompressed() const
{
    return (m_pack_method != 0x30);
}

const byte *
FileBlock::data()
{
    if( m_data == NULL )
        m_data = getData();

    return m_data;
}

byte *
FileBlock::getData()
{
    byte * data = new byte[packSize()];

    size_t old_pos = m_stream.tellg();

    m_stream.seekg(m_base + m_header_size);
    m_stream.read( reinterpret_cast<char *>(data), m_added_size);
    m_stream.seekg(old_pos, std::ios::beg);

    return data;
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

wstring
FileBlock::extraDebugRepr() const
{
    std::wstringstream stream;
    stream<<"[isDir]\t\t "<<isDir()<<"\n";
    stream<<"[HostOS]\t "<<(uint16)m_host_os<<"\n";
    stream<<"[useUnicode]\t "<<useUnicode()<<"\n";
    stream<<"[hasSalt]\t "<<hasSalt()<<"\n";
    stream<<"[hasPassword]\t "<<hasPassword()<<"\n";
    stream<<"[hasExtTime]\t "<<hasExtTime()<<"\n";
    stream<<"\n";
    stream<<"[filename]\t"<<filename()<<"\n";

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

    // change path separator: from M$ style '\' to Unix style '/'
    std::replace( m_filename,
                  m_filename + sizeof(m_filename)/sizeof(m_filename[0]),
                  L'\\',
                  L'/'
                 );

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
    // TODO; implement it, or not?
}

bool
FileBlock::isMissingPart() const
{
    return m_flags & 0x0001;
}

bool
FileBlock::needMissingPart() const
{
    return m_flags & 0x0002U;
}

bool
FileBlock::hasPassword() const
{
    return m_flags & 0x0004U;
}

bool
FileBlock::hasEmbededComment() const
{
    return m_flags & 0x0008U;
}

bool
FileBlock::isSolid() const
{
    return m_flags & 0x0010U;
}

bool
FileBlock::sizeOfDictInKB() const
{
    //FIXME
    //mask = 0x00e0U
    //result = m_flags & mask
    return 64;
}

bool
FileBlock::isDir() const
{
    return (m_flags & 0x00e0U) == 0x00e0U ;
}

bool
FileBlock::isLargeFile() const
{
    return m_flags & 0x0100U;
}

bool
FileBlock::useUnicode() const
{
    return m_flags & 0x0200U;
}

bool
FileBlock::hasSalt() const
{
    return m_flags & 0x0400U;
}

bool
FileBlock::hasExtTime() const
{
    return m_flags & 0x1000U;
}

bool
FileBlock::hasExtFlags() const
{
    return m_flags & 0x2000U;
}
