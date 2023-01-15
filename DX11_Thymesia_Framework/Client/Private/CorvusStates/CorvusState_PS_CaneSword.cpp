#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_CaneSword.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Weapon.h"
#include "Effect_Decal.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_CaneSword);
CLONE_C(CCorvusState_PS_CaneSword, CComponent)

void CCorvusState_PS_CaneSword::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_CaneSword::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 96:
		GET_SINGLE(CGameManager)->Deactivate_Zoom(0.1f, EASING_TYPE::LINEAR);
		return;
	case 97: 
	{
		_float3 vPlayerPos;
		ZeroMemory(&vPlayerPos, sizeof(_float3));
		XMStoreFloat3(&vPlayerPos, GET_SINGLE(CGameManager)->Get_PlayerPos());
		GAMEINSTANCE->Set_RadialBlur(0.2f, vPlayerPos);

		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		XMStoreFloat4x4(&m_DecalDesc.WorldMatrix, OwnerWorldMatrix);

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);
	}
		return;
	case 100:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
	}
		return;
	case 174:
		TurnOff_Effect("Corvus_PW_EyeGlow_Special");
		return;
	case 191:
		TurnOff_Effect("Corvus_PW_CaneSword_SP02_Weapon");
		return;
	}
}

HRESULT CCorvusState_PS_CaneSword::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_CaneSword::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_CaneSword::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_CaneSword_SP02");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_CaneSword::Call_AnimationEnd, this, placeholders::_1);

	m_DecalDesc.vScale = { 3.f,7.129f,0.1f };
	m_DecalDesc.vPosition = { -0.16f, 0.f, 3.78f, 1.f };
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 2.f;
	m_DecalDesc.vColor = _float3(1.f, 1.f, 1.f);
	m_DecalDesc.strTextureTag = "DecalLinear";

}

void CCorvusState_PS_CaneSword::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_CaneSword::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

_bool CCorvusState_PS_CaneSword::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 209)
	{
		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}
	else
	{
		Rotation_TargetToLookDir();
	}

	return false;
}


void CCorvusState_PS_CaneSword::OnStateStart(const _float& In_fAnimationBlendTime)
{
	CPlayerStateBase::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_CaneSword::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_CaneSword::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_CaneSword::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_CaneSword::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_CaneSword::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_CaneSword::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_CaneSword::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_CaneSword::Call_AnimationEnd, this, placeholders::_1);
}
