#include "fs.h"

#include <sstream>
#include <iostream>


#include "archive.h"
#include "fileblock.h"
#include "fileentry.h"
#include "direntry.h"

#include <sys/stat.h>
static const wstring ROOT = L"/";

FileSystem::FileSystem (const char * archive_name):
m_archive_name(archive_name),
m_root(NULL)
{
    m_archive = new Archive(archive_name);

    // special code for setting up a fake root
    m_root = new DirEntry( ROOT );
    m_dirs.push_back(m_root);
    m_dirs2[ROOT] = m_root;

    parse();
    parse2();
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


void
FileSystem::treenize()
{
    map<wstring, FileEntry *>::const_iterator f_iter;
    for( f_iter = m_files2.begin(); f_iter != m_files2.end(); f_iter++)
    {
        wstring key = f_iter->second->dirname();
        map<wstring, DirEntry *>::const_iterator dir;
        dir = m_dirs2.find(key);

        if( dir == m_dirs2.end() )
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
    for( d_iter = m_dirs2.begin(); d_iter != m_dirs2.end(); d_iter++)
    {
        if (d_iter->second == m_root)
        {
            continue;
        }

        wstring key = d_iter->second->dirname();

        map<wstring, DirEntry *>::const_iterator dir;
        dir = m_dirs2.find(key);

        if( dir == m_dirs2.end() )
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
        FileEntry * entry = new FileEntry(fileblocks[i]->filename() );
        entry->addBlock(fileblocks[i]);
        m_files.push_back(entry);
    }

    vector<FileBlock *> dirblocks = m_archive->dirBlocks();
    size = dirblocks.size();
    for ( size_t i = 0; i < size; i ++ )
    {
        DirEntry * entry = new DirEntry(dirblocks[i]->filename() );
        entry->addBlock(dirblocks[i]);
        m_dirs.push_back(entry);
    }

}


wstring
FileSystem::debugRepr() const
{
    std::wstringstream stream;

    stream<<"Rar FileSystem\n";
    stream<<"\n";

    stream<<"Files:\n";
    size_t size = m_files.size();
    for (size_t i = 0; i < size; i++ )
        stream<<m_files[i]->debugRepr();
    stream<<"\n";

    stream<<"Dirs:\n";
    size = m_dirs.size();
    for (size_t i = 0; i < size; i++ )
        stream<<m_dirs[i]->debugRepr();
    stream<<"\n";

    return stream.str();
}



void
FileSystem::parse2()
{
    vector<FileBlock *> fileblocks = m_archive->fileBlocks();

    size_t size = fileblocks.size();
    for ( size_t i = 0; i < size; i ++ )
    {
        FileBlock * block = fileblocks[i];
        wstring name = block->filename();


        FileEntry * f_entry = NULL;
        map<wstring, FileEntry *>::iterator iter ;
        iter = m_files2.find(name);

        if( iter == m_files2.end() )
        {
            f_entry = new FileEntry(name);
            f_entry->addBlock(block);
            m_files2[name]=f_entry;

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
        iter = m_dirs2.find(name);

        if( iter == m_dirs2.end() )
        {
            d_entry = new DirEntry(name);
            d_entry->addBlock(block);
            m_dirs2[name]=d_entry;

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

    for( f_iter=m_files2.begin(); f_iter != m_files2.end() ; f_iter++)
    {
        stream<<f_iter->second->debugRepr();
    }
    stream<<"\n";


    stream<<"Dirs:\n";
    map<wstring, DirEntry *>::const_iterator d_iter;
    for(d_iter=m_dirs2.begin(); d_iter != m_dirs2.end() ; d_iter++)
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

