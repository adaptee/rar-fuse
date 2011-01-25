#include <sstream>
#include "mainblock.h"
#include "assert.h"

MainBlock::MainBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    assert( m_type == 0x73) ;
}

string
MainBlock::extraDebugRepr()
{
    std::stringstream stream;
    stream<<"[hasPassword]\t "<<hasPassword()<<"\n";
    stream<<"[isVolume]\t "<<isVolume()<<"\n";
    stream<<"[isFirstVolume]\t "<<isFirstVolume()<<"\n";

    return stream.str();
}

bool
MainBlock::isVolume()
{
    return m_flags & 0x0001;
}

bool
MainBlock::hasEmbededComment()
{
    return m_flags & 0x0002;
}

bool
MainBlock::isLocked()
{
    return m_flags & 0x0004;
}

bool
MainBlock::isSolid()
{
    return m_flags & 0x0008;
}

//scheme xxxx.partN.rar
bool
MainBlock::useNewSchemeForVolumeName()
{
    return m_flags & 0x0010;
}


bool
MainBlock::hasAuthInfo()
{
    return m_flags & 0x0020;
}

bool
MainBlock::hasRecoveryRecord()
{
    return m_flags & 0x0040;
}

bool
MainBlock::hasPassword()
{
    return m_flags & 0x0080;
}

bool
MainBlock::isFirstVolume()
{
    return m_flags & 0x0100;
}

bool
MainBlock::encryptVersion()
{
    return m_flags & 0x0200;
}

