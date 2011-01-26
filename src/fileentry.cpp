#include "fileentry.h"
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

