#ifndef ARCHIVE_GUARD
#define ARCHIVE_GUARD

#include <fstream>
#include <string>
#include <vector>

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

    std::string debugRepr() const;


private:
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

