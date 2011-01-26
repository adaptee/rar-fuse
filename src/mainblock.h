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
    bool isVolume() const ;
    bool hasEmbededComment() const ;
    bool isLocked() const ;
    bool isSolid() const ;
    bool useNewSchemeForVolumeName() const ;
    bool hasAuthInfo() const ;
    bool hasRecoveryRecord() const ;
    bool hasPassword() const ;
    bool isFirstVolume() const ;
    bool encryptVersion() const ;;

};


#endif /* end of include guard: MAINBLOCK_GUARD */
