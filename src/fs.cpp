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

