#include <assert.h>

#include "entry.h"
#include "fileblock.h"

const wchar_t UNIX_SEPARATOR = L'/';


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


wstring
Entry::basename() const
{
    size_t pos = m_name.find_last_of(UNIX_SEPARATOR);

    // name does not contain any path separator
    if (pos == wstring::npos)
    {
        return m_name;
    }
    else
    {
        return m_name.substr(pos+1, wstring::npos);
    }

}

wstring
Entry::dirname()  const
{
    size_t pos = m_name.find_last_of(UNIX_SEPARATOR);

    // name does not contain any path separator
    if (pos == wstring::npos)
    {
        return wstring(L"/");
    }
    else
    {
        return m_name.substr(0, pos);
    }

}




