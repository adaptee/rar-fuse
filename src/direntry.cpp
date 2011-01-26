#include "direntry.h"
#include <sstream>

DirEntry::DirEntry( const string & name): Entry(name)
{


}

string
DirEntry::debugRepr() const
{
    std::stringstream stream;
    stream<<"[dir] "<<name()<<"\n";

    size_t size = m_subentries.size();
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

