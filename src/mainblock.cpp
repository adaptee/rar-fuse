#include "mainblock.h"

#include <sstream>
#include <assert.h>

MainBlock::MainBlock(std::ifstream & stream, size_t base): Block(stream, base)
{
    assert( m_type == 0x73) ;
}

wstring
MainBlock::extraDebugRepr()
{
    std::wstringstream stream;
    stream<<"[hasPassword]\t "<<hasPassword()<<"\n";
    stream<<"[isVolume]\t "<<isVolume()<<"\n";
    stream<<"[isFirstVolume]\t "<<isFirstVolume()<<"\n";

    return stream.str();
}

bool
MainBlock::isVolume() const
{
    return m_flags & 0x0001U;
}

bool
MainBlock::hasEmbededComment() const
{
    return m_flags & 0x0002U;
}

bool
MainBlock::isLocked() const
{
    return m_flags & 0x0004U;
}

bool
MainBlock::isSolid() const
{
    return m_flags & 0x0008U;
}

//scheme xxxx.partN.rar
bool
MainBlock::useNewSchemeForVolumeName() const
{
    return m_flags & 0x0010U;
}


bool
MainBlock::hasAuthInfo() const
{
    return m_flags & 0x0020U;
}

bool
MainBlock::hasRecoveryRecord() const
{
    return m_flags & 0x0040U;
}

bool
MainBlock::hasPassword() const
{
    return m_flags & 0x0080U;
}

bool
MainBlock::isFirstVolume() const
{
    return m_flags & 0x0100U;
}

bool
MainBlock::encryptVersion() const
{
    return m_flags & 0x0200U;
}

