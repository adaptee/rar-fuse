#ifndef ENTRY_GUARD
#define ENTRY_GUARD

#include <string>
using std::wstring;
#include <vector>
using std::vector;

class FileBlock;

class Entry
{
public:
    Entry(const wstring & name);

    wstring name()   const { return m_name; }
    wstring basename() const ;
    wstring dirname()  const ;

    Entry * parent() const { return m_parent; }
    void setParent(Entry * parent) { m_parent = parent; }

    void addBlock( FileBlock * block);


    virtual bool isFile()      const =0 ;
    virtual bool isDir()       const =0 ;
    virtual size_t size()      const =0 ;
    virtual wstring debugRepr() const =0 ;

protected:
    wstring m_name;
    Entry * m_parent;
    vector<FileBlock *> m_blocks;

};

#endif /* end of include guard: ENTRY_GUARD */
