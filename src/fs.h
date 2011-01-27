#ifndef FS_GUARD
#define FS_GUARD

#include <vector>
using std::vector;
#include <string>
using std::wstring;

class Archive;
class FileEntry;
class DirEntry;

class FileSystem
{
public:
    FileSystem (const char * archive_name);
    wstring debugRepr() const;
    Archive * archive() const;

private:
    void parse();

    Archive *    m_archive;
    const char * m_archive_name;

    DirEntry * m_root;
    vector< FileEntry * > m_files;
    vector< DirEntry  * > m_dirs;

}; // end of class FileSystem

#endif /* end of include guard: FS_GUARD */
