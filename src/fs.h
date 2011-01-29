#ifndef FS_GUARD
#define FS_GUARD

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::wstring;

class Archive;
class Entry;
class FileEntry;
class DirEntry;

struct stat;
//#include <sys/stat.h>

class FileSystem
{
public:
    FileSystem (const char * archive_name);
    ~FileSystem();

    Archive * archive() const;

    wstring debugRepr() const;
    wstring debugRepr2() const;
    wstring debugRepr3() const;

    const struct stat * getStatus( const wstring & name) const;

private:
    void parse();
    void parse2();
    void treenize();

    Entry * getEntry(const wstring & name) const;
    Entry * getFile(const wstring & name) const;
    Entry * getDir(const wstring & name) const;


    const char *    m_archive_name;
    Archive *       m_archive;
    DirEntry *      m_root;

    vector< FileEntry * > m_files;
    vector< DirEntry  * > m_dirs;

    map< wstring, FileEntry * > m_files2;
    map< wstring, DirEntry * > m_dirs2;
    map< wstring, Entry * > m_entries;


}; // end of class FileSystem

#endif /* end of include guard: FS_GUARD */
