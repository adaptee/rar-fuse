#include "direntry.h"
#include <sstream>

DirEntry::DirEntry( const wstring & name): Entry(name)
{


}

const struct stat *
DirEntry::status()
{
    // FIXME; currently we only support READ-ONLY filesystem
    m_stat.st_mode  = S_IFDIR | 0555 ;
    m_stat.st_size  = size();
    m_stat.st_nlink = 2 + m_subentries.size();
    // FIXME; use fake value here;
    m_stat.st_atime = 0x999999 ;
    m_stat.st_mtime = 0x999999 ;

    return &m_stat;

}

vector<wstring>
DirEntry::read() const
{
    vector<wstring> names;

    vector< Entry * >::const_iterator iter;
    for( iter = m_subentries.begin(); iter != m_subentries.end() ; iter++)
        names.push_back( (*iter)->basename() );
        //names.push_back( (*iter)->name() );

    return names;

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

