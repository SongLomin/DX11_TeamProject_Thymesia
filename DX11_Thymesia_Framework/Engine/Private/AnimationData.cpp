#include "AnimationData.h"

HRESULT ANIMATION_DATA::Make_AnimationData(aiAnimation* In_pAiAnimation, _float In_fSpeed)
{
    szName = In_pAiAnimation->mName.data;

    iNumChannels = In_pAiAnimation->mNumChannels;

    fDuration = (_float)In_pAiAnimation->mDuration;
    fTickPerSecond = (_float)In_pAiAnimation->mTicksPerSecond * In_fSpeed;

    // TODO : !!! DO NOT ERASE !!!
// Temporary Macro for changing animation speed.
// undefined after use.
#define SET_ANIM_SPEED(AnimationName, WantedSpeed)\
    if (!strcmp(szName.c_str(), AnimationName))\
    {\
        fTickPerSecond *= WantedSpeed;\
    }

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
#pragma region Urd

    SET_ANIM_SPEED("Armature|Armature|Urd_Attack01LV2|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack01|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack02LV2C0|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack02LV2C1|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack02|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack03_DashSting_L|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack04_DashSting_R|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack05|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack06|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Attack07|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Dead|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Equip_L|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Equip_R|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_FightStart|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_HurtCounter|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_HurtM_FL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_HurtM_FR|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_HurtS_FL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_HurtS_FR|BaseLayerL", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Idle|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Parry_L|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Parry_RQuick|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Parry_R|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Run|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Seq_BossFightStart_1|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Seq_TutorialBossFightStart|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Skill01|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Skill02_1|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Skill03_L|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_Skill03_R|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_SPSkill01|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StepB|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StepFL45|BaseLayer", 2.f);;
    SET_ANIM_SPEED("Armature|Armature|Urd_StepFL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StepFR45|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StepFR|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StepL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StepR|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StunEnd|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StunLoop|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_StunStart|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_VS_TakeExecution|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkBL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkBR|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkB|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkFL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkFR|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkF|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkL|BaseLayer", 2.f);
    SET_ANIM_SPEED("Armature|Armature|Urd_WalkR|BaseLayer", 2.f);
#pragma endregion // Urd

#pragma region Mon_AxeMan
    SET_ANIM_SPEED("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunStart|BaseLaye", 2.f);
#pragma endregion // Mon_AxeMan
#undef SET_ANIM_SPEED

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
