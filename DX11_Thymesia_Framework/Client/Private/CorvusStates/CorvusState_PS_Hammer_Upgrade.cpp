#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Hammer_Upgrade.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CCorvusState_PS_Hammer_Upgrade);
CLONE_C(CCorvusState_PS_Hammer_Upgrade, CComponent)

HRESULT CCorvusState_PS_Hammer_Upgrade::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Hammer_Upgrade::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_PS_Hammer_Upgrade::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Hammer_B_Ver2");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Hammer_Upgrade::Call_AnimationEnd, this);
}

void CCorvusState_PS_Hammer_Upgrade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_PS_Hammer_Upgrade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_PS_Hammer_Upgrade::OnDisable()
{

}

void CCorvusState_PS_Hammer_Upgrade::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_Hammer_Upgrade::Call_NextKeyFrame, this, placeholders::_1);
}

void CCorvusState_PS_Hammer_Upgrade::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_Hammer_Upgrade::Call_NextKeyFrame, this, placeholders::_1);
}

void CCorvusState_PS_Hammer_Upgrade::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_PS_Hammer_Upgrade::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 39:
	{
		GET_SINGLE(CGameManager)->Store_EffectIndex("Corvus_PW_EyeGlow", GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_PW_EyeGlow", m_pTransformCom, _uint(TIMESCALE_LAYER::PLAYER)));
	}
	return;
	case 71:
	{
		GAMEINSTANCE->Set_MotionBlur(0.15f);
	}
	return;
	case 127:
	{
		GAMEINSTANCE->Set_MotionBlur(0.15f);
	}
	return;
	case 239:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.3f);
	}
	return;
	case 284:
	{
		GET_SINGLE(CGameManager)->UnUse_EffectGroup("Corvus_PW_EyeGlow", GET_SINGLE(CGameManager)->Get_StoredEffectIndex("Corvus_PW_EyeGlow"));
	}
	return;
	}
}



void CCorvusState_PS_Hammer_Upgrade::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_PS_Hammer_Upgrade::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Hammer_Upgrade::Call_AnimationEnd, this);
}

_bool CCorvusState_PS_Hammer_Upgrade::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

