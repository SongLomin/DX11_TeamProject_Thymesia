#pragma once
#include "Base.h"

BEGIN(Engine)

struct ENGINE_DLL BONE_DATA
{
    string			                    szName;

    _float4x4                           OffsetMatrix;

    HRESULT Make_BoneData(aiBone* In_pAIBone);

public:
    void Bake_Binary(ofstream& os);
    void Load_FromBinary(ifstream& is);

};

END