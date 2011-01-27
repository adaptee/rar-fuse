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
    Archive * archive() const;
    wstring debugRepr() const;

    wstring debugRepr2() const;
    wstring debugRepr3() const;

private:
    void parse();
    void parse2();

    void treenize();


    Archive *    m_archive;
    const char * m_archive_name;

    DirEntry * m_root;
    vector< FileEntry * > m_files;
    vector< DirEntry  * > m_dirs;

    map< wstring, FileEntry * > m_files2;
    map< wstring, DirEntry * > m_dirs2;


}; // end of class FileSystem

#endif /* end of include guard: FS_GUARD */
