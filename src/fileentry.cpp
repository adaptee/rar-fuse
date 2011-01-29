#include "fileentry.h"

#include <sstream>
#include "fileblock.h"

FileEntry::FileEntry( const wstring & name): Entry(name)
{
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
    m_stat.st_mode  = S_IFDIR | 0444 ;
    m_stat.st_size  = size();
    m_stat.st_nlink = 1;
    // FIXME; use fake value here;
    m_stat.st_atime = 0x999999 ;
    m_stat.st_mtime = 0x999999 ;

    return &m_stat;
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
