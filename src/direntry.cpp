#include "direntry.h"
#include <sstream>

DirEntry::DirEntry( const wstring & name): Entry(name)
{


}

wstring
DirEntry::debugRepr() const
{
    std::wstringstream stream;
    stream<<"[dir] "<<name()<<"\n";
    //stream<<"[dir] "<<basename()<<"\n";
    //stream<<"[dir] "<<dirname()<<"\n";

    size_t size = m_subentries.size();
    stream<<"[subentries]"<<size<<"\n";
    for( size_t i = 0; i < size; i++ )
        stream<<m_subentries[i]->debugRepr();

    return stream.str();

}


vector< Entry *>
DirEntry::subEntries() const
{
    return m_subentries;
}

void DirEntry::addSubEntry(Entry * subentry)
{
    m_subentries.push_back(subentry);
}

