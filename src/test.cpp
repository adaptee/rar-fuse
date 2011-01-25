#include <fstream>
#include <iostream>
using std::ios;
using std::cout;

#include "defs.h"
#include "block.h"
#include "markerblock.h"
#include "mainblock.h"
#include "fileblock.h"
#include "subblock.h"
#include "endblock.h"

Block * buildblock(std::ifstream & file, size_t base)
{
    // make sure we always read from the right point
    file.seekg(base, std::ios::beg);

    // read-ahead to fetch type of next block, then move readptr back.
    char buf[4];
    file.read(buf, 3);
    file.seekg (-3, std::ios::cur);

    byte type = buf[2];
    switch(type)
    {
        case 0x72:
            return new MarkerBlock(file, base);
            break;
        case 0x73:
            return new MainBlock(file, base);
            break;
        case 0x74:
            return new FileBlock(file, base);
            break;
        case 0x7a:
            return new SubBlock(file, base);
            break;
        case 0x7b:
            return new EndBlock(file, base);
            break;
        default:
            return NULL;
    };
}

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

int main(int argc, char const* argv[])
{
    size_t base = 0;
    std::ifstream file(argv[1], std::ios::in|std::ios::binary);

    size_t length = get_leng_of_stream(file);

    while( base < length )
    {
        Block * block = buildblock(file, base);
        std::cout<<block->debugRepr();

        base += block->totalSize();
    }

    return 0;
}
