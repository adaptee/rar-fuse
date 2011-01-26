#include "fileentry.h"

FileEntry::FileEntry( const wstring & name): Entry(name)
{

}

size_t
FileEntry::size() const
{
    return 1;
}

