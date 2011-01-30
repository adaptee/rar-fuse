#include "fs.h"

#include <sstream>
#include <iostream>

#include <sys/stat.h>

#include "archive.h"
#include "fileentry.h"
#include "direntry.h"
#include "fileblock.h"
#include "descriptor.h"

static const wstring ROOT = L"/";

FileSystem::FileSystem (const char * archive_name):
m_archive_name(archive_name),
m_root(NULL)
{
    m_archive = new Archive(archive_name);

    // special code for setting up a fake root
    m_root = new DirEntry( ROOT );
    m_dirs[ROOT] = m_root;
    m_entries[ROOT] = m_root;

    parse();
    treenize();
}

FileSystem::~FileSystem()
{
    delete m_archive;
    delete m_root;
}

Archive *
FileSystem::archive() const
{
    return m_archive;
}

const struct stat *
FileSystem::getStatus( const wstring & name) const
{
    Entry * entry = getEntry(name);
    if(entry)
        return entry->status();
    else
        return static_cast<const struct stat *>( NULL);
}


vector<wstring>
FileSystem::readDir(const wstring & name) const
{
    DirEntry * entry = getDir(name);
    if(entry)
    {
        return entry->readDir() ;
    }
    else
    {
        vector<wstring> names ;
        return names ;
    }
}

size_t
FileSystem::readFile(const wstring & name,
                    void * dest, size_t offset, size_t count) const

{
    FileEntry * entry = getFile(name);

    if(entry)
    {
        Descriptor descriptor(entry, 0);
        return descriptor.read(dest, offset, count);
    }
    else
    {
        return 0;
    }

}

Entry *
FileSystem::getEntry(const wstring & name) const
{
    Entry * entry = NULL;

    map<wstring, Entry *>::const_iterator iter;
    iter = m_entries.find(name);

    if( iter != m_entries.end() )
        entry = iter->second ;

    return entry;

}

FileEntry *
FileSystem::getFile(const wstring & name) const
{
    FileEntry * entry = NULL;

    map<wstring, FileEntry *>::const_iterator iter;
    iter = m_files.find(name);

    if( iter != m_files.end() )
        entry = iter->second ;

    return entry;
}


DirEntry *
FileSystem::getDir(const wstring & name) const
{
    DirEntry * entry = NULL;

    map<wstring, DirEntry *>::const_iterator iter;
    iter = m_dirs.find(name);

    if( iter != m_dirs.end() )
        entry = iter->second ;

    return entry;
}

void
FileSystem::treenize()
{
    map<wstring, FileEntry *>::const_iterator f_iter;
    for( f_iter = m_files.begin(); f_iter != m_files.end(); f_iter++)
    {
        wstring key = f_iter->second->dirname();
        map<wstring, DirEntry *>::const_iterator dir;
        dir = m_dirs.find(key);

        if( dir == m_dirs.end() )
        {
            ;
        }
        else
        {
            dir->second->addSubEntry( f_iter->second);
            f_iter->second->setParent(dir->second);
        }
    }

    map<wstring, DirEntry *>::const_iterator d_iter;
    for( d_iter = m_dirs.begin(); d_iter != m_dirs.end(); d_iter++)
    {
        if (d_iter->second == m_root)
        {
            continue;
        }

        wstring key = d_iter->second->dirname();

        map<wstring, DirEntry *>::const_iterator dir;
        dir = m_dirs.find(key);

        if( dir == m_dirs.end() )
        {
            //FIXME; shoud raise some exception?
            ;
        }
        else
        {
            dir->second->addSubEntry( d_iter->second);
            d_iter->second->setParent(dir->second);
        }
    }

}


void
FileSystem::parse()
{
    vector<FileBlock *> fileblocks = m_archive->fileBlocks();

    size_t size = fileblocks.size();
    for ( size_t i = 0; i < size; i ++ )
    {
        FileBlock * block = fileblocks[i];
        wstring name = block->filename();


        FileEntry * f_entry = NULL;
        map<wstring, FileEntry *>::iterator iter ;
        iter = m_files.find(name);

        if( iter == m_files.end() )
        {
            f_entry = new FileEntry(name);
            f_entry->addBlock(block);
            m_files[name]=f_entry;
            m_entries[name]=f_entry;

        }
        else
        {
            f_entry = iter->second;
            f_entry->addBlock(block);
        }

    }

    vector<FileBlock *> dirblocks = m_archive->dirBlocks();

    size = dirblocks.size();
    for ( size_t i = 0; i < size; i ++ )
    {
        FileBlock * block = dirblocks[i];
        wstring name = block->filename();

        DirEntry * d_entry = NULL;
        map<wstring, DirEntry *>::iterator iter ;
        iter = m_dirs.find(name);

        if( iter == m_dirs.end() )
        {
            d_entry = new DirEntry(name);
            d_entry->addBlock(block);
            m_dirs[name]=d_entry;

            m_entries[name]=d_entry;
        }
        else
        {
            d_entry = iter->second;
            d_entry->addBlock(block);
        }

    }

}

wstring
FileSystem::debugRepr2() const
{

    std::wstringstream stream;

    stream<<"Rar FileSystem\n";
    stream<<"\n";

    stream<<"Files:\n";
    map<wstring, FileEntry * >::const_iterator f_iter;

    for( f_iter=m_files.begin(); f_iter != m_files.end() ; f_iter++)
    {
        stream<<f_iter->second->debugRepr();
    }
    stream<<"\n";


    stream<<"Dirs:\n";
    map<wstring, DirEntry *>::const_iterator d_iter;
    for(d_iter=m_dirs.begin(); d_iter != m_dirs.end() ; d_iter++)
    {
        stream<<d_iter->second->debugRepr();
    }
    stream<<"\n";

    return stream.str();

}

wstring
FileSystem::debugRepr3() const
{
    std::wstringstream stream;
    stream<<m_root->debugRepr();
    return stream.str();
}


wstring
FileSystem::debugRepr4() const
{

    std::wstringstream stream;

    stream<<"Entry names:";
    stream<<"\n";

    map<wstring, Entry * >::const_iterator f_iter;

    for( f_iter=m_entries.begin(); f_iter != m_entries.end() ; f_iter++)
    {
        stream<<f_iter->first;
        stream<<"\n";
    }
    stream<<"\n";

    return stream.str();
}

