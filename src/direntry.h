#ifndef DIRENTRY_GUARD
#define DIRENTRY_GUARD
#include "entry.h"

class DirEntry: public Entry
{
public:
    DirEntry( const wstring & name);

    virtual bool isFile() const { return false; }
    virtual bool isDir()  const { return true;  }
    virtual size_t size() const { return 4094;  }
    virtual wstring debugRepr() const ;

    virtual const struct stat * status() ;
    vector<wstring> readDir() const;

    void addSubEntry(Entry * subentry);


private:

    vector< Entry *> m_subentries ;
    vector< Entry *> m_subdirentries ;


}; // end of class DirEntry


#endif /* end of include guard: DIRENTRY_GUARD */
