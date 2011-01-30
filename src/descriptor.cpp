#include "descriptor.h"

#include "fileentry.h"

Descriptor::Descriptor( const FileEntry * entry, size_t fd):
m_fd(fd),
m_entry(entry),
m_size(entry->size() ),
m_offset(0)
{

}

size_t
Descriptor::read( void * dest, size_t count)
{
    // do nothing if that is what we were told of
    if( count == 0 )
        return 0;

    // end of file ?
    if( m_offset >= m_size)
    {
        return 0;
    }

    size_t real_count = count;
    if ( m_offset + count > m_size)
        real_count = (m_size - m_offset) ;

    // FIXME
    // here we assume FileEntry::read() will always work perfectly.
    m_entry->read(dest, m_offset, real_count);
    return real_count;
}

bool
Descriptor::eof() const
{
    return (m_offset >= m_size);
}

