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
    wstring debugRepr4() const;

    const struct stat * getStatus( const wstring & name) const;
    vector<wstring> readDir(const wstring & name) const;

    size_t readFile(const wstring & name,
                     void * dest, size_t offset, size_t count) const;

private:
    void parse();
    void treenize();

    Entry * getEntry(const wstring & name) const;
    FileEntry * getFile(const wstring & name) const;
    DirEntry * getDir(const wstring & name) const;


    const char *    m_archive_name;
    Archive *       m_archive;
    DirEntry *      m_root;

    map< wstring, FileEntry * > m_files;
    map< wstring, DirEntry * > m_dirs;
    map< wstring, Entry * > m_entries;


}; // end of class FileSystem

#endif /* end of include guard: FS_GUARD */
