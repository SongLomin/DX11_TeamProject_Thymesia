#pragma once
#include "Base.h"
#include "NvCloth/Callbacks.h"
class CCustomPxAssertHandler :
    public nv::cloth::PxAssertHandler
{
    // PxAssertHandler을(를) 통해 상속됨
    virtual void operator()(const char* exp, const char* file, int line, bool& ignore) override;
};

