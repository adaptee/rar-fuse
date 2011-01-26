#include "entry.h"

Entry::Entry(const wstring & name):
m_name(name),
m_parent(NULL)
{

}

void
Entry::addBlock( FileBlock * block)
{
    m_blocks.push_back(block);
}



