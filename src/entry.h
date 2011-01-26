#ifndef ENTRY_GUARD
#define ENTRY_GUARD

#include <string>
using std::wstring;

class Entry
{
public:
    Entry(const wstring & name);

    wstring name() const { return m_name; }

    virtual bool isFile() const =0 ;
    virtual bool isDir()  const =0 ;

protected:
    wstring m_name;
};

#endif /* end of include guard: ENTRY_GUARD */
