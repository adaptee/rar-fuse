#ifndef ARCHIVE_GUARD
#define ARCHIVE_GUARD

#include <fstream>
#include <string>
#include <vector>
using std::vector;

class Block;
class MarkerBlock;
class MainBlock;
class FileBlock;
class SubBlock;
class EndBlock;


class Archive
{

public:
    Archive( const char * filename);
    ~Archive();

    vector< FileBlock *> fileBlocks () const;
    vector< FileBlock *> dirBlocks () const;

    std::wstring debugRepr() const;


private:

    // make copy-constructor and copy-assignment un-usable
    Archive( const Archive & block);
    void operator =(const Archive & block);

    void parse();

    const char * m_filename;
    std::ifstream m_file;

    MarkerBlock * m_markerblock;
    MainBlock   * m_mainblock;
    EndBlock    * m_endblock;

    std::vector<Block     * >  m_blocks ;
    std::vector<FileBlock * >  m_fileblocks;
    std::vector<FileBlock * >  m_dirblocks;
    std::vector<SubBlock  * >  m_subblocks;


}; // end of class Archive


#endif /* end of include guard: ARCHIVE_GUARD */

