#include "fileentry.h"

#include <sstream>

#include <assert.h>
#include <string.h>

#include "fileblock.h"

FileEntry::FileEntry( const wstring & name):
Entry(name),
m_data(NULL),
m_rawData(NULL)
{
}

FileEntry::~FileEntry()
{
    delete[] m_data ;
    delete[] m_rawData ;
}

size_t
FileEntry::size() const
{
    size_t sum = 0;

    size_t blocknum = m_blocks.size();
    for( size_t i =0; i < blocknum; i++ )
	sum += m_blocks[i]->unpackSize();

    return sum;
}

size_t
FileEntry::rawSize() const
{
    size_t sum = 0;

    size_t blocknum = m_blocks.size();
    for( size_t i =0; i < blocknum; i++ )
	sum += m_blocks[i]->packSize();

    return sum;
}

bool
FileEntry::isCompressed() const
{
    return m_blocks[0]->isCompressed();
}

const struct stat *
FileEntry::status()
{
    // FIXME; currently we only support READ-ONLY filesystem
    m_stat.st_mode  = S_IFREG | 0444 ;
    m_stat.st_size  = size();
    m_stat.st_nlink = 1;

    // extra precision ( to 100 nanoseconds) of time means
    // nothing in *nix system .So. just use normal time.
    m_stat.st_atime = time() ;
    m_stat.st_ctime = time() ;
    m_stat.st_mtime = time() ;

    //m_stat.st_atime = atime() ;
    //m_stat.st_ctime = ctime() ;
    //m_stat.st_mtime = mtime();

    return &m_stat;
}

size_t
FileEntry::read( void * dest, size_t offset, size_t count)
{
    assert ( offset + count <=  size() );

    memcpy( dest, data() + offset, count);

    return 0;
}

byte *
FileEntry::data()
{
    if( m_data == NULL)
        m_data = getData();

    return m_data;
}

byte *
FileEntry::getData()
{
    if( ! isCompressed() )
    {
        return rawData();
    }
    else
    {
        return NULL;
    }
}

byte *
FileEntry::rawData()
{
    if( m_rawData == NULL)
        m_rawData = getRawData();

    return m_rawData;
}

byte *
FileEntry::getRawData()
{
    byte * rawdata = new byte[rawSize()];

    byte * dest = rawdata;

    vector<FileBlock *>::const_iterator iter;
    for( iter = m_blocks.begin(); iter != m_blocks.end() ; iter++)
    {
        FileBlock * block = (*iter);
        const byte * src = block->data();
        memcpy( dest, src, block->packSize());

        dest += block->packSize();
    }

    return rawdata;
}

wstring
FileEntry::debugRepr() const
{
    std::wstringstream stream;

    stream<<"[file]";
    stream<<"\t"<<name();
    stream<<"\t"<<size()<<" bytes";
    stream<<"\n";

    return stream.str();

}
