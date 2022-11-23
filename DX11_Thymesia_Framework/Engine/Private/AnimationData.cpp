#include "AnimationData.h"

HRESULT ANIMATION_DATA::Make_AnimationData(aiAnimation* In_pAiAnimation, _float In_fSpeed)
{
    szName = In_pAiAnimation->mName.data;

    iNumChannels = In_pAiAnimation->mNumChannels;

    /* 현재 애니메이션을 재상하는데 걸리는 시간. */
    fDuration = (_float)In_pAiAnimation->mDuration;
    fTickPerSecond = (_float)In_pAiAnimation->mTicksPerSecond * In_fSpeed;



    for (_uint i = 0; i < iNumChannels; i++)
    {
        shared_ptr<CHANNEL_DATA> pChannelData = make_shared<CHANNEL_DATA>();
        pChannelData->Make_ChannelData(In_pAiAnimation->mChannels[i]);
        Channel_Datas.push_back(pChannelData);
    }

    return S_OK;
}

void ANIMATION_DATA::Bake_Binary(ofstream& os)
{
    write_typed_data(os, szName.size());
    os.write(&szName[0], szName.size());

    write_typed_data(os, iNumChannels);
    write_typed_data(os, fDuration);
    write_typed_data(os, fTickPerSecond);

    for (_uint i = 0; i < iNumChannels; i++)
    {
        Channel_Datas[i]->Bake_Binary(os);
    }
}

void ANIMATION_DATA::Load_FromBinary(ifstream& is)
{
    size_t istringSize;
    read_typed_data(is, istringSize);
    szName.resize(istringSize);
    is.read(&szName[0], istringSize);

    read_typed_data(is, iNumChannels);
    read_typed_data(is, fDuration);
    read_typed_data(is, fTickPerSecond);

#define MACRO(AnimationName, WantedSpeed)\
    if (!strcmp(szName.c_str(), AnimationName))\
    {\
        fTickPerSecond *= WantedSpeed;\
    }

    // TODO : change animation speed here
    MACRO("SK_C_Corvus.ao|Corvus_SD_LAttack1", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD_LAttack2", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD_LAttack3_New", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD1_ParryL_NEW", 4.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD1_ParryR_NEW", 4.f);


    MACRO("SK_C_Corvus.ao|Corvus_Raven_ClawLong_L01", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_Raven_ClawLong_L02", 2.f);
#undef MACRO


    for (_uint i = 0; i < iNumChannels; i++)
    {
        shared_ptr<CHANNEL_DATA> pChannelData = make_shared<CHANNEL_DATA>();
        pChannelData->Load_FromBinary(is);
        Channel_Datas.push_back(pChannelData);
    }

}
