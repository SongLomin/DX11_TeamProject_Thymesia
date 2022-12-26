#include "AnimationData.h"

HRESULT ANIMATION_DATA::Make_AnimationData(aiAnimation* In_pAiAnimation, _float In_fSpeed)
{
    szName = In_pAiAnimation->mName.data;

    iNumChannels = In_pAiAnimation->mNumChannels;

    fDuration = (_float)In_pAiAnimation->mDuration;
    fTickPerSecond = (_float)In_pAiAnimation->mTicksPerSecond * In_fSpeed;

    for (_uint i(0); i < iNumChannels; i++)
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
    // Temporary Macro for changing animation speed.
    // undefined after use.
#define SET_ANIM_SPEED(AnimationName, WantedSpeed)\
    if (!strcmp(szName.c_str(), AnimationName))\
    {\
        fTickPerSecond *= WantedSpeed;\
    }

    // TODO : change animation speed here
#pragma region Corvus
   /*SET_ANIM_SPEED("Corvus_SD_LAttack1", 2.5f);
   SET_ANIM_SPEED("Corvus_SD_LAttack2", 2.5f);
   SET_ANIM_SPEED("Corvus_SD_LAttack3_New", 2.5f);
   SET_ANIM_SPEED("Corvus_SD1_ParryL_NEW", 4.f);
   SET_ANIM_SPEED("Corvus_SD1_ParryR_NEW", 4.f);
   SET_ANIM_SPEED("Corvus_VSVarg_TakeSPAttack", 1.853f);


	SET_ANIM_SPEED("Corvus_Raven_ClawLong_L01", 2.f);
	SET_ANIM_SPEED("Corvus_Raven_ClawLong_L02", 2.f);
	SET_ANIM_SPEED("Corvus_Raven_ClawShortV2_Fast01", 2.f);
	SET_ANIM_SPEED("Corvus_Raven_ClawShortV2_Fast02", 2.f);
	SET_ANIM_SPEED("Corvus_Raven_ClawShortV2_Fast03", 2.f);
	SET_ANIM_SPEED("Corvus_Raven_ClawLong_PlunderAttack2TTT", 2.f);
	SET_ANIM_SPEED("Corvus_SD1_Fall_Attack", 2.5f);

    SET_ANIM_SPEED("Corvus_SD_AvoidShort_F", 1.75f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShort_L", 1.75f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShort_R", 1.75f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShort_B_NEW", 1.75f);

    SET_ANIM_SPEED("Corvus_SD_AvoidLongB_NEW", 1.5f);
    SET_ANIM_SPEED("Corvus_SD_AvoidLongF", 1.5f);
    SET_ANIM_SPEED("Corvus_SD_AvoidLongL_NEW", 1.5f);
    SET_ANIM_SPEED("Corvus_SD_AvoidLongR_NEW", 1.5f);

    SET_ANIM_SPEED("Corvus_SD_AvoidF", 2.25f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShortMag_B", 2.25f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShortMag_F", 2.25f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShortMag_L", 2.25f);
    SET_ANIM_SPEED("Corvus_SD_AvoidShortMag_R", 2.25f);
    SET_ANIM_SPEED("Corvus_SD1_Sprint", 2.f);
    SET_ANIM_SPEED("Corvus_SD1_Sprint_Start", 2.f);
    SET_ANIM_SPEED("Corvus_SD_SprintAttack_1", 2.5f);
    SET_ANIM_SPEED("Corvus_Raven_ClawCommonV2_ChargeStart", 1.25f);
    SET_ANIM_SPEED("Corvus_Raven_ClawCommon_ChargeLoop", 1.5f);
    SET_ANIM_SPEED("Corvus_Raven_ClawLong_ChargeFull", 2.f);
    SET_ANIM_SPEED("Corvus_SD_TD2Thorw2", 1.75f);
    SET_ANIM_SPEED("Corvus_PW_Axe", 2.f);
    SET_ANIM_SPEED("Corvus_PW_Axe_B_V1", 2.f);
    SET_ANIM_SPEED("Corvus_PW_Knife_A_V1", 2.75f);

    SET_ANIM_SPEED("Corvus_PW_VargSword_B", 1.75f);
    SET_ANIM_SPEED("Corvus_PW_VargSword_Start", 1.75f);

    SET_ANIM_SPEED("Corvus_PW_UrdSword01", 2.25f);
    SET_ANIM_SPEED("Corvus_PW_Magician_A", 1.75f);
    SET_ANIM_SPEED("Corvus_PW_CaneSword_SP02", 2.f);
    SET_ANIM_SPEED("Corvus_SD_LAttack4", 2.5f);
    SET_ANIM_SPEED("Corvus_SD_LAttack5", 2.5f);
    SET_ANIM_SPEED("Corvus_SD_AvoidAttack_D", 3.5f);
    SET_ANIM_SPEED("Corvus_SD_AvoidAttack_E2", 2.5f);

    SET_ANIM_SPEED("Corvus_SD_ParryDeflect_L", 2.f);
    SET_ANIM_SPEED("Corvus_SD_ParryDeflect_LUp", 2.f);
    SET_ANIM_SPEED("Corvus_SD_HurtXLF", 2.f);
    SET_ANIM_SPEED("Corvus_SD_HurtXXLF", 2.f);
    SET_ANIM_SPEED("Corvus_SD_HurtKnockDown", 2.f);
    SET_ANIM_SPEED("Corvus_SD_HurtFallDown_End", 2.f);
    SET_ANIM_SPEED("Corvus_SD_HurtFallDown", 2.f);

	SET_ANIM_SPEED("Corvus_SD_ParryDeflect_R", 2.f);
	SET_ANIM_SPEED("Corvus_SD_ParryDeflect_RUp", 2.f);

	SET_ANIM_SPEED("Corvus_SD_Ladder_Climb_L_UP", 2.f);
	SET_ANIM_SPEED("Corvus_SD_Ladder_Climb_R_UP", 2.f);
	SET_ANIM_SPEED("Corvus_SD_Ladder_Climb_L_Down", 2.f);
	SET_ANIM_SPEED("Corvus_SD_Ladder_Climb_R_Down", 2.f);

	SET_ANIM_SPEED("Corvus_SD_VSVarg_Execution", 1.6f);
	SET_ANIM_SPEED("Corvus_VSBossBat_Execution01", 1.4f);

	SET_ANIM_SPEED("Corvus_StunExecute_StartL_L", 7.f);
	SET_ANIM_SPEED("Corvus_SD_TD2Thorw2", 2.5f);
	SET_ANIM_SPEED("Corvus_SD_VSVarg_Execution_Blending", 1.1f);*/

#pragma region Varg
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_SPAttack1_Catch", 2.306f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_SPAttack1_Fail", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_SPAttack1_Roar", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_SPAttack1_Roar_GetUp", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack1_1", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack1_2", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack1_3", 1.25f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack2_1", 2.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack2_2", 2.5f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack3_1", 2.5f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack2_2b", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack2_2b1", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack2_2b2", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_ComboAttack2_3b", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TurnAttack1", 2.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TakeExecution_Start_FIX", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TakeParry_FIX", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TakeExecution_Dead", 1.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TakeExecution_End", 1.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TakeExecution_Loop", 1.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TakeExecution_NoDeadEnd", 1.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_Seq_TutorialBossFightStart", 5.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_Seq_BossFightStart", 6.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_SPAttack1_CatchSeq1", 2.5f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_AvoidB", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_AvoidAttack1", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_RaidAttack2", 2.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_RunF_Start", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_RunF_1", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_RunF", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TurnL90", 1.5f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_TurnR90", 1.5f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_WalkB", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_WalkF", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_WalkL", 1.25f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_WalkR", 1.25f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_StunLoop", 2.f);
    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_StunEnd", 2.f);

    SET_ANIM_SPEED("SK_C_Varg.ao|Varg_Dead", 1.25f);
#pragma endregion // Varg

#pragma region Joker
    SET_ANIM_SPEED("Joker_ComboA01", 1.25f);
    SET_ANIM_SPEED("Joker_ComboA02", 1.25f);
    SET_ANIM_SPEED("Joker_ComboB01", 1.25f);
    SET_ANIM_SPEED("Joker_ComboB02", 1.25f);
    SET_ANIM_SPEED("Joker_JumpAttack", 1.25f);
    SET_ANIM_SPEED("Joker_ShockAttack", 1.25f);
    SET_ANIM_SPEED("Joker_Stun_Start", 1.25f);
    SET_ANIM_SPEED("Joker_Stun_Loop", 1.25f);
    SET_ANIM_SPEED("Joker_Stun_End", 1.25f);
    SET_ANIM_SPEED("Joker_TurnAttackL", 1.25f);
    SET_ANIM_SPEED("Joker_TurnAttackR", 1.25f);
    SET_ANIM_SPEED("Joker_TurnL90", 1.25f);
    SET_ANIM_SPEED("Joker_TurnR90", 1.25f);
    SET_ANIM_SPEED("Joker_WheelAttackStart", 1.25f);
    SET_ANIM_SPEED("Joker_WheelAttackLoop", 1.25f);
    SET_ANIM_SPEED("Joker_WheelAttackEnd", 1.25f);
    SET_ANIM_SPEED("Joker_TakeExecution_Start", 1.4f);
#pragma endregion // Joker

#pragma region Bat
    SET_ANIM_SPEED("BossBat_Seq_BossFightStart_V1", 4.f);
    SET_ANIM_SPEED("BossBat_AttackL_01_1", 4.f);
    SET_ANIM_SPEED("BossBat_AttackL_01_2a", 4.f);
    SET_ANIM_SPEED("BossBat_AttackL_01_2b", 4.f);
    SET_ANIM_SPEED("BossBat_AttackL_01_3a", 4.f);
    SET_ANIM_SPEED("BossBat_AttackR_01_1", 2.f);
    SET_ANIM_SPEED("BossBat_AttackR_01_2a", 4.f);
    SET_ANIM_SPEED("BossBat_AttackR_01_2b", 4.f);
    SET_ANIM_SPEED("BossBat_Bite_1", 4.f);
    SET_ANIM_SPEED("BossBat_Bite_2", 4.f);
    SET_ANIM_SPEED("BossBat_Car", 4.f);
    SET_ANIM_SPEED("BossBat_Charge", 4.f);
    SET_ANIM_SPEED("BossBat_FTurn_L", 1.f);
    SET_ANIM_SPEED("BossBat_FTurn_R", 4.f);
    SET_ANIM_SPEED("BossBat_Hellscream", 4.f);
    SET_ANIM_SPEED("BossBat_HurtXL_F", 4.f);
    SET_ANIM_SPEED("BossBat_HurtXL_L", 4.f);
    SET_ANIM_SPEED("BossBat_JumpSmash_Chest", 2.f);
    SET_ANIM_SPEED("BossBat_JumpSmashForwardL", 4.f);
    SET_ANIM_SPEED("BossBat_JumpSmashL", 4.f);
    SET_ANIM_SPEED("BossBat_SonicBoom", 4.f);
    SET_ANIM_SPEED("BossBat_SP01", 2.f);
    SET_ANIM_SPEED("BossBat_Storm_1", 4.f);
    SET_ANIM_SPEED("BossBat_StunEnd", 4.f);
    SET_ANIM_SPEED("BossBat_StunLoop", 1.3f);
    SET_ANIM_SPEED("BossBat_StunStart", 1.3f);
    SET_ANIM_SPEED("BossBat_TakeExecution_End", 1.2f);
    SET_ANIM_SPEED("BossBat_TakeExecution_Loop", 1.2f);
    SET_ANIM_SPEED("BossBat_TakeExecution_Start01", 1.46f);
    SET_ANIM_SPEED("BossBat_TurnL90", 2.f);
    SET_ANIM_SPEED("BossBat_TurnR90", 2.f);
    SET_ANIM_SPEED("BossBat_WalkF", 2.f);


#pragma endregion // Bat

#pragma region Mon_AxeMan
    SET_ANIM_SPEED("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunStart|BaseLaye", 2.f);
#pragma endregion // Mon_AxeMan

#undef SET_ANIM_SPEED

    for (_uint i(0); i < iNumChannels; i++)
    {
        shared_ptr<CHANNEL_DATA> pChannelData = make_shared<CHANNEL_DATA>();
        pChannelData->Load_FromBinary(is);
        Channel_Datas.push_back(pChannelData);
    }
}

void ANIMATION_DATA::Bake_ReverseAnimation(shared_ptr<ANIMATION_DATA>& Out_AnimationData)
{

    Out_AnimationData->szName = szName + "_Reverse";

    Out_AnimationData->iNumChannels = iNumChannels;

    Out_AnimationData->fDuration = fDuration;
    Out_AnimationData->fTickPerSecond = fTickPerSecond;


    for (_uint i = 0; i < iNumChannels; i++)
    {
        shared_ptr<CHANNEL_DATA> pChannelData = make_shared<CHANNEL_DATA>();
        Channel_Datas[i]->Bake_ReverseChannel(pChannelData, fDuration);
        Out_AnimationData->Channel_Datas.push_back(pChannelData);
    }
}
