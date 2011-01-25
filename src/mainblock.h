#ifndef MAINBLOCK_GUARD
#define MAINBLOCK_GUARD

#include "block.h"

class MainBlock: public Block
{
public:
    MainBlock(std::ifstream & stream, size_t base);

protected:
    virtual string extraDebugRepr();

    // predicators based upon flags
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


#endif /* end of include guard: MAINBLOCK_GUARD */
