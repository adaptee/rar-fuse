#include "fileentry.h"

#include <sstream>

#include <assert.h>
#include <string.h>

#include "fileblock.h"

FileEntry::FileEntry( const wstring & name):
Entry(name),
m_data(NULL)
{
}

FileEntry::~FileEntry()
{
    delete[] m_data ;
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

const struct stat *
FileEntry::status()
{
    // FIXME; currently we only support READ-ONLY filesystem
    m_stat.st_mode  = S_IFREG | 0444 ;
    m_stat.st_size  = size();
    m_stat.st_nlink = 1;
    // FIXME; use fake value here;
    m_stat.st_atime = 0x999999 ;
    m_stat.st_mtime = 0x999999 ;

    return &m_stat;
}

size_t
FileEntry::read( void * dest, size_t offset, size_t count)
{
    if ( ! m_data)
        getData();

    assert ( offset + count <=  size() );

    memcpy( dest, m_data + offset, count);

    return 0;
}

void
FileEntry::getData()
{
    if( !m_data)
    m_data = new byte[size()];

    byte * dest = m_data;

    vector<FileBlock *>::const_iterator iter;
    for( iter = m_blocks.begin(); iter != m_blocks.end() ; iter++)
    {
        FileBlock * block = (*iter);
        const byte * src = block->data();
        memcpy( dest, src, block->unpackSize());

        dest += block->unpackSize();
    }

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
