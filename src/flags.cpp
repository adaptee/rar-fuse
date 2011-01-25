#include "flags.h"

Flags::Flags(uint16 hex): m_hex(hex)
{

}

Flags::~Flags()
{

}


bool
Flags::shouldSkipUnknownBlock()
{
    return m_hex & 0x4000 ;
}

bool
Flags::hasAddedSize()
{
    return m_hex & 0x8000;
}

MainFlags::MainFlags(uint16 hex): Flags(hex)
{

}

bool
MainFlags::isVolume()
{
    return m_hex & 0x0001;
}

bool
MainFlags::hasEmbededComment()
{
    return m_hex & 0x0002;
}

    bool
MainFlags::isLocked()
{
    return m_hex & 0x0004;
}

    bool
MainFlags::isSolid()
{
    return m_hex & 0x0008;
}

//scheme xxxx.partN.rar
bool
MainFlags::useNewSchemeForVolumeName()
{
    return m_hex & 0x0010;
}


bool
MainFlags::hasAuthInfo()
{
    return m_hex & 0x0020;
}

bool
MainFlags::hasRecoveryRecord()
{
    return m_hex & 0x0040;
}

bool
MainFlags::hasPassword()
{
    return m_hex & 0x0080;
}

bool
MainFlags::isFirstVolume()
{
    return m_hex & 0x0100;
}

bool
MainFlags::encryptVersion()
{
    return m_hex & 0x0200;
}

FileFlags::FileFlags(uint16 hex): Flags(hex)
{
}

bool
FileFlags::isMissingPart()
{
    return m_hex & 0x0001;
}

bool
FileFlags::needMissingPart()
{
    return m_hex & 0x0002;
}

bool
FileFlags::hasPassword()
{
    return m_hex & 0x0004;
}

bool
FileFlags::hasEmbededComment()
{
    return m_hex & 0x0008;
}

bool
FileFlags::isSolid()
{
    return m_hex & 0x0010;
}

bool
FileFlags::sizeOfDictInKB()
{
    //FIXME
    //mask = 0x00e0
    //result = m_hex & mask
    return 64;
}

bool
FileFlags::isDir()
{
    return m_hex & 0x00e0 == 0x00e0;
}

bool
FileFlags::isLargeFile()
{
    return m_hex & 0x0100;
}

bool
FileFlags::useUnicode()
{
    return m_hex & 0x0200;
}

bool
FileFlags::hasSalt()
{
    return m_hex & 0x0400;
}

bool
FileFlags::hasExtTime()
{
    return m_hex & 0x1000;
}

bool
FileFlags::hasExtFlags()
{
    return m_hex & 0x2000;
}



