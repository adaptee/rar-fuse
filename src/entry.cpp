#include <assert.h>

#include "entry.h"
#include "fileblock.h"

Entry::Entry(const wstring & name):
m_name(name),
m_parent(NULL)
{

}

void
Entry::addBlock( FileBlock * block)
{
    // you do not want to add blocks belongint to others!
    if ( m_blocks.size() > 0 )
        assert( block->filename() == m_blocks.back()->filename() );

    m_blocks.push_back(block);
}



