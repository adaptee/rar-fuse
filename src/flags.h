#ifndef FLAGS_GUARD
#define FLAGS_GUARD
#include "defs.h"

class Flags
{
public:
    Flags(uint16 hex);

    bool shouldSkipUnknownBlock();
    bool hasAddedSize();

protected:
    uint16 m_hex;

};

class MainFlags: public Flags
{
public:
    MainFlags(uint16 hex);

    bool isVolume();
    bool hasEmbededComment();
    bool isLocked();
    bool isSolid();
    bool useNewSchemeForVolumeName();
    bool hasAuthInfo();
    bool hasRecoveryRecord();
    bool hasPassword();
    bool isFirstVolume();
    bool encryptVersion();;

};

class FileFlags: public Flags
{
public:
    FileFlags(uint16 hex);

    bool isMissingPart();
    bool needMissingPart();
    bool hasPassword();
    bool hasEmbededComment();
    bool isSolid();
    bool sizeOfDictInKB();
    bool isDir();
    bool isLargeFile();
    bool useUnicode();
    bool hasSalt();
    bool hasExtTime();
    bool hasExtFlags();
};


#endif /* end of include guard: FLAGS_GUARD */
