#include "fs.h"

#include <sstream>

#include "archive.h"
#include "fileblock.h"
#include "fileentry.h"
#include "direntry.h"

FileSystem::FileSystem (const char * archive_name):
m_archive_name(archive_name),
m_root(NULL)
{
    m_archive = new Archive(archive_name);
    parse();
    parse2();
}

Archive *
FileSystem::archive() const
{
    return m_archive;
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

        map<wstring, FileEntry *>::iterator iter ;
        iter = m_files2.find(name);

        if( iter == m_files2.end() )
        {
            FileEntry * entry = new FileEntry(name);
            entry->addBlock(block);
            m_files2[name]=entry;
        }
        else
        {
           iter->second->addBlock(block);
        }

    }

    vector<FileBlock *> dirblocks = m_archive->dirBlocks();

    size = dirblocks.size();
    for ( size_t i = 0; i < size; i ++ )
    {
        FileBlock * block = dirblocks[i];
        wstring name = block->filename();

        map<wstring, DirEntry *>::iterator iter ;
        iter = m_dirs2.find(name);

        if( iter == m_dirs2.end() )
        {
            DirEntry * entry = new DirEntry(name);
            entry->addBlock(block);
            m_dirs2[name]=entry;
        }
        else
        {
           iter->second->addBlock(block);
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
