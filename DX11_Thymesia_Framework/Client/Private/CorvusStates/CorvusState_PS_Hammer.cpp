#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Hammer.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Effect_Decal.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_Hammer);
CLONE_C(CCorvusState_PS_Hammer, CComponent)

void CCorvusState_PS_Hammer::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_Hammer::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 50:
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	case 102:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.3f);

		XMStoreFloat4x4(&m_DecalDesc.WorldMatrix, OwnerWorldMatrix);

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);

	}
	return;
	case 155:
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_Hammer::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Hammer::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_Hammer::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Hammer_A");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Hammer::Call_AnimationEnd, this, placeholders::_1);

	m_DecalDesc.vScale = { 5.f,5.f,0.1f };
	m_DecalDesc.vPosition = { -0.137f,0.f,1.656f, 1.f };
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 2.f;
	m_DecalDesc.vColor = _float3(0.f, 1.f, 0.7f);
	m_DecalDesc.strTextureTag = "DecalTexture";
}

void CCorvusState_PS_Hammer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_Hammer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

_bool CCorvusState_PS_Hammer::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 167)
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



void CCorvusState_PS_Hammer::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_Hammer::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_Hammer::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Hammer::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_Hammer::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_Hammer::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Hammer::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_Hammer::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Hammer::Call_AnimationEnd, this, placeholders::_1);
}
