#include <assert.h>
#include <string.h>

#include "entry.h"
#include "fileblock.h"
#include "dostime.h"

const wchar_t UNIX_SEPARATOR = L'/';
static const wstring ROOT    = L"/";


Entry::Entry(const wstring & name):
m_name(name),
m_parent(NULL)
{
    memset((void*)&m_stat, 0x0, sizeof(m_stat));
}


void
Entry::addBlock( FileBlock * block)
{
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

    // the name does not contain any path separator,
    // or the name is the root: "/"
    if ((pos == wstring::npos) || (pos == 0))
    {
        return ROOT;
    }
    else
    {
        return m_name.substr(0, pos);
    }

}


time_t
Entry::time() const
{
    dostime_t dostime = m_blocks[0]->time();
    return dos_to_unix_time(dostime);
}
