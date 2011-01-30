#ifndef DESCRIPTOR_GUARD
#define DESCRIPTOR_GUARD

// for size_t
#include <stddef.h>

#include "defs.h"

class FileEntry;

class Descriptor
{
public:
    Descriptor( FileEntry * entry, size_t fd);
    ~Descriptor();

    size_t read( void * dest, size_t offset, size_t count) ;

private:

    FileEntry * const m_entry;
    const size_t m_fd;
    const size_t m_size;

    size_t m_offset;

};



#endif /* end of include guard: DESCRIPTOR_GUARD */
