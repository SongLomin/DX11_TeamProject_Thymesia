#include "stdafx.h"
#include "Skill.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "SkillChain.h"
#include "SkillBall.h"
#include "FadeMask.h"
#include "LuxiyaStates/LuxiyaStates.h"

GAMECLASS_C(CSkill);
CLONE_C(CSkill, CComponent);

HRESULT CSkill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkill::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    USE_START(CSkill)

    return S_OK;
}

void CSkill::Start()
{
    m_pOwner.lock()->Get_Component<CSkillChain>().lock()->Register_Skill(Cast<CSkill>(m_this));
    m_pOwnerFromCharacter = Weak_Cast<CCharacter>(m_pOwner);
}

void CSkill::Call_Do_Skill(_uint iSkillChainCnt, _bool& Out_bActive)
{
    if (!Check_Requirements())
    {
        Out_bActive = false;
        return;
    }

    if (!m_pOwnerFromCharacter.lock()->Get_CurState().lock()->Check_Requirement())
    {
        Out_bActive = false;
        return;
    }

    
#ifdef _DEBUG
    cout << "스킬 발동" << endl;
#endif    
    

    if (Out_bActive && m_pSkillStateCom.lock())
    {
        m_pOwnerFromCharacter.lock()->Change_State(m_pSkillStateCom);
    }


    Out_bActive = true;
}

void CSkill::Set_BallTexture(const _char* sKey, _uint index)
{
    m_sBallTextureKey = sKey;
    m_iBallTextureIndex = index;
}

void CSkill::Bind_SkillState(weak_ptr<CStateBase> pSkillStateCom)
{
    m_pSkillStateCom = pSkillStateCom;

}

_bool CSkill::Check_Requirements()
{
    _bool bIsUsable = true;

    CallBack_Requirements(bIsUsable);

    return bIsUsable;
}

weak_ptr<CSkillBall> CSkill::Add_SkillBall(const _float& In_fPositionX)
{
    weak_ptr<CSkillBall> pSkillBall = GAMEINSTANCE->Add_GameObject<CSkillBall>(LEVEL::LEVEL_GAMEPLAY);
    pSkillBall.lock()->Init_PositionX(In_fPositionX);
    pSkillBall.lock()->Set_Texture(m_sBallTextureKey);
    pSkillBall.lock()->Set_TextureIndex(m_iBallTextureIndex);
    pSkillBall.lock()->CallBack_Skill += bind(&CSkill::Call_Do_Skill, this, placeholders::_1, placeholders::_2);

    return pSkillBall;
}

void CSkill::Free()
{
}
