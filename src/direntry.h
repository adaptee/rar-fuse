#ifndef DIRENTRY_GUARD
#define DIRENTRY_GUARD
#include "entry.h"

class DirEntry: public Entry
{
public:
    DirEntry( const string & name);

    virtual bool isFile() const { return false; }
    virtual bool isDir()  const { return true;  }
    virtual size_t size() const { return 4094;  }
    virtual string debugRepr() const ;

    vector< Entry *> subEntries() const ;
    void addSubEntry(Entry * subentry);


private:
    vector< Entry *> m_subentries ;

}; // end of class DirEntry


#endif /* end of include guard: DIRENTRY_GUARD */
