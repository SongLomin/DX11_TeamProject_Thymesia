#pragma once

#include "Base.h"

BEGIN(Engine)
struct ENGINE_DLL CHANNEL_DATA
{
    


    string                              szNodeName;
    _uint                               iNumScalingKeys;
    _uint                               iNumRotationKeys;
    _uint                               iNumPositionKeys;

    vector<KEY_DATA>                    tScalingKeys;
    vector<ROTATIONKEY_DATA>            tRotationKeys;
    vector<KEY_DATA>                    tPositionKeys;

    HRESULT Make_ChannelData(aiNodeAnim* In_pChannel);

public:
    void Bake_Binary(ofstream& os);
    void Load_FromBinary(ifstream& is);

};
END