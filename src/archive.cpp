#include <sstream>

#include "archive.h"
#include "block.h"
#include "markerblock.h"
#include "mainblock.h"
#include "fileblock.h"
#include "subblock.h"
#include "endblock.h"

// becasuse we do not have ifstream.length(),
// we have to do a trcik to know its length.
size_t get_leng_of_stream( std::ifstream & stream)
{
    size_t old_pos = stream.tellg();

    stream.seekg(0, std::ios::end);
    size_t length = stream.tellg();
    // do not forget to restore the old position.
    stream.seekg(old_pos, std::ios::beg);

    return length;
}

Archive::Archive( const char * filename) :
m_filename(filename),
m_file(filename, std::ios::in|std::ios::binary)
{
    parse();
}


// class Archive should be regarded as the owner of all Blocks
// since all blocks are created in Archive::parse().
// So it is natually also its duty to destory those blocks.
Archive::~Archive()
{

    delete m_markerblock;
    delete m_mainblock;
    delete m_endblock;

    size_t size = m_fileblocks.size();
    for( size_t i=0; i < size; i++)
	delete m_fileblocks[i];

    size = m_dirblocks.size();
    for( size_t i=0; i < size; i++)
	delete m_dirblocks[i];

    size = m_subblocks.size();
    for( size_t i=0; i < size; i++)
	delete m_subblocks[i];
}


void
Archive::parse()
{
    size_t base = 0;
    size_t length = get_leng_of_stream(m_file);

    Block * curr_block = NULL;
    char peek[4];

    while( base < length )
    {
        // make sure we always read from the right point
        m_file.seekg(base, std::ios::beg);

        // read-ahead to fetch type of next block, then move readptr back.
        m_file.read(peek, 3);
        m_file.seekg (-3, std::ios::cur);

        byte type = peek[2];
        switch(type)
        {
            case 0x72:
                m_markerblock  = new MarkerBlock(m_file, base);
                curr_block = (Block *) m_markerblock;
                break;

            case 0x73:
                m_mainblock = new MainBlock(m_file, base);
                curr_block = (Block *) m_mainblock;
                break;

            case 0x74:
                FileBlock * fileblock ;
                fileblock = new FileBlock(m_file, base) ;
                if (fileblock->isDir())
                    m_dirblocks.push_back(fileblock);
                else
                    m_fileblocks.push_back(fileblock);

                //m_fileblocks.push_back( new FileBlock(m_file, base) );
                curr_block = (Block *) fileblock;

                break;

            case 0x7a:
                m_subblocks.push_back(new SubBlock(m_file, base));
                curr_block = (Block *) m_subblocks.back();
                break;

            case 0x7b:
                m_endblock = new EndBlock(m_file, base);
                curr_block = (Block *) m_endblock;
                break;

            default:
                curr_block = new Block(m_file, base);
        };

        m_blocks.push_back(curr_block);
        base += curr_block->totalSize();

        curr_block = NULL ;

    }
}


std::string
Archive::debugRepr() const
{
    std::stringstream stream;

    size_t size = m_blocks.size();
    for(int i=0; i < size; i++)
        stream<< m_blocks[i]->debugRepr();

    return stream.str();
}


vector< FileBlock *>
Archive::fileBlocks () const
{
    return m_fileblocks;
}

vector< FileBlock *>
Archive::dirBlocks () const
{
    return m_dirblocks;
}

