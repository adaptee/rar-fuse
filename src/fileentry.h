#ifndef FILEENTRY_GUARD
#define FILEENTRY_GUARD

#include "entry.h"
#include "defs.h"

class FileEntry: public Entry
{
public:
    FileEntry( const wstring & name);
    ~FileEntry();

    virtual bool isFile() const { return true;}
    virtual bool isDir()  const { return false;}
    virtual size_t size() const;
    virtual wstring debugRepr() const ;

    virtual const struct stat * status() ;
    size_t read( void * dest, size_t offset, size_t count) ;
private:

    bool isCompressed() const;
    size_t rawSize() const;

    void getData();

    //byte * data();
    //byte * getData();

    //byte * rawData();
    //byte * getRawData();

    byte * m_data ;
    byte * m_rawData ;

};


#endif /* end of include guard: FILEENTRY_GUARD */
