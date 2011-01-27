#ifndef FS_GUARD
#define FS_GUARD

#include <vector>
using std::vector;
#include <map>
using std::map;
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

    wstring debugRepr2() const;
private:
    void parse();
    void parse2();

    Archive *    m_archive;
    const char * m_archive_name;

    DirEntry * m_root;
    vector< FileEntry * > m_files;
    vector< DirEntry  * > m_dirs;

    map< wstring, FileEntry * > m_files2;
    map< wstring, DirEntry * > m_dirs2;


}; // end of class FileSystem

#endif /* end of include guard: FS_GUARD */
