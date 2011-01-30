#ifndef FILEENTRY_GUARD
#define FILEENTRY_GUARD

#include "entry.h"

class FileEntry: public Entry
{
public:
    FileEntry( const wstring & name);

    virtual bool isFile() const { return true;}
    virtual bool isDir()  const { return false;}
    virtual size_t size() const;
    virtual wstring debugRepr() const ;

    virtual const struct stat * status() ;
    size_t read( void * dest, size_t offset, size_t count) const;

};


#endif /* end of include guard: FILEENTRY_GUARD */
