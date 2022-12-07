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

    // TODO : !!! DO NOT ERASE !!!
#define MACRO(AnimationName, WantedSpeed)\
    if (!strcmp(szName.c_str(), AnimationName))\
    {\
        fTickPerSecond *= WantedSpeed;\
    }

    // TODO : change animation speed here
    MACRO("Corvus_SD_LAttack1", 2.5f);
    MACRO("Corvus_SD_LAttack2", 2.5f);
    MACRO("Corvus_SD_LAttack3_New", 2.5f);
    MACRO("Corvus_StunExecute_StartL_L", 3.f);
    MACRO("Corvus_SD1_ParryL_NEW", 4.f);
    MACRO("Corvus_SD1_ParryR_NEW", 4.f);
    MACRO("Corvus_VSVarg_TakeSPAttack", 1.853f);


    MACRO("Corvus_Raven_ClawLong_L01", 2.f);
    MACRO("Corvus_Raven_ClawLong_L02", 2.f);

    MACRO("Corvus_SD_RunF_24", 1.5f);
    MACRO("Corvus_SD_AvoidF", 2.25f);
    MACRO("Corvus_SD1_Sprint", 2.25f);
    MACRO("Corvus_SD1_Sprint_Start", 2.25f);
    MACRO("Corvus_Raven_ClawCommonV2_ChargeStart", 1.25f);
    MACRO("Corvus_Raven_ClawCommon_ChargeLoop", 1.5f);
    MACRO("Corvus_Raven_ClawLong_ChargeFull", 1.75f);
    MACRO("Corvus_SD_TD2Thorw2", 1.75f);
    MACRO("Corvus_PW_Axe", 3.f);
    MACRO("Corvus_PW_Knife_A_V1", 2.75f);
    MACRO("Corvus_PW_VargSword_B", 2.5f);
    MACRO("Corvus_PW_UrdSword01", 2.25f);
    MACRO("Corvus_PW_Magician_A", 1.75f);
    MACRO("Corvus_PW_CaneSword_SP02", 2.f);
    MACRO("Corvus_SD_LAttack4", 2.5f);
    MACRO("Corvus_SD_LAttack5", 2.5f);
    MACRO("Corvus_SD_AvoidAttack_D", 2.5f);
    MACRO("Corvus_SD_AvoidAttack_E2", 2.5f);
    MACRO("Corvus_SD1_Fall_Attack", 2.5f);

    MACRO("Corvus_SD_ParryDeflect_L", 2.25f);
    MACRO("Corvus_SD_ParryDeflect_LUp", 2.25f);

    MACRO("Corvus_SD_ParryDeflect_R", 2.25f);
    MACRO("Corvus_SD_ParryDeflect_RUp", 2.25f);


    MACRO("SK_C_Varg.ao|Varg_Seq_BossFightStart", 5.5f);
    MACRO("SK_C_Varg.ao|Varg_SPAttack1_Catch", 2.306f);

    MACRO("SK_C_Varg.ao|Varg_ComboAttack1_1", 1.25f);
    MACRO("SK_C_Varg.ao|Varg_ComboAttack1_2", 1.25f);
    MACRO("SK_C_Varg.ao|Varg_ComboAttack1_3", 1.25f);

    MACRO("SK_C_Varg.ao|Varg_ComboAttack2_1", 2.5f);
    MACRO("SK_C_Varg.ao|Varg_ComboAttack2_2", 2.5f);

    MACRO("SK_C_Varg.ao|Varg_ComboAttack3_1", 2.5f);

    MACRO("SK_C_Varg.ao|Varg_ComboAttack2_2b", 2.f);
    MACRO("SK_C_Varg.ao|Varg_ComboAttack2_2b1", 2.f);
    MACRO("SK_C_Varg.ao|Varg_ComboAttack2_2b2", 2.f);
    MACRO("SK_C_Varg.ao|Varg_ComboAttack2_3b", 2.f);
    MACRO("SK_C_Varg.ao|Varg_TurnAttack1", 2.5f);

    MACRO("SK_C_Varg.ao|Varg_AvoidB", 1.25f);
    MACRO("SK_C_Varg.ao|Varg_AvoidR", 1.25f);
    MACRO("SK_C_Varg.ao|Varg_AvoidL", 1.25f);

    MACRO("SK_C_Corvus.ao|Corvus_SD_AvoidShort_F", 1.25f);


    MACRO("SK_C_Corvus.ao|Corvus_SD_Ladder_Climb_L_UP", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD_Ladder_Climb_R_UP", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD_Ladder_Climb_L_Down", 2.f);
    MACRO("SK_C_Corvus.ao|Corvus_SD_Ladder_Climb_R_Down", 2.f);


#undef MACRO

    for (_uint i = 0; i < iNumChannels; i++)
    {
        shared_ptr<CHANNEL_DATA> pChannelData = make_shared<CHANNEL_DATA>();
        pChannelData->Load_FromBinary(is);
        Channel_Datas.push_back(pChannelData);
    }

}
