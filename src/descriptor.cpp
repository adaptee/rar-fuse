#include "descriptor.h"

#include "fileentry.h"

Descriptor::Descriptor( FileEntry * entry, size_t fd):
m_fd(fd),
m_entry(entry),
m_size(entry->size() ),
m_offset(0)
{

}

Descriptor::~Descriptor()
{
}

size_t
Descriptor::read( void * dest, size_t offset, size_t count)
{
    // do nothing if that is what we were told of
    if( count == 0 )
        return 0;

    // end of file ?
    if( offset >= m_size)
    {
        return 0;
    }

    size_t real_count = count;
    if ( offset + count > m_size)
        real_count = (m_size - offset) ;

    // FIXME
    // assume FileEntry::read() will always work perfectly.
    m_entry->read(dest, offset, real_count);

    return real_count;
}



