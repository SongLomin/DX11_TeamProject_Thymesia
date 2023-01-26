#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_VargSword.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Effect_Decal.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_VargSword);
CLONE_C(CCorvusState_PS_VargSword, CComponent)

void CCorvusState_PS_VargSword::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_VargSword::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 41:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(-1.f, -0.2f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.2f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.2f);
	}
	return;
	case 97:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.3f);

		m_DecalDesc.vPosition = { -0.157f,0.f,2.121f, 1.f };
		XMStoreFloat4(&m_DecalDesc.vPosition, XMVector3TransformCoord(XMLoadFloat4(&m_DecalDesc.vPosition), OwnerWorldMatrix));

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);
	}
	return;
	case 120:
		TurnOff_Effect("Corvus_PW_VargSword_Weapon");
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_VargSword::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_VargSword::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_DecalDesc.vScale = { 3.f,6.129f, 0.1f};
	m_DecalDesc.vPosition = { -0.157f,0.f,2.121f, 1.f };
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 2.f;
	m_DecalDesc.vColor = _float3(0.f, 1.f, 0.7f);
	m_DecalDesc.strTextureTag = "DecalLinear";

	return S_OK;
}

void CCorvusState_PS_VargSword::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_VargSword_B");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_VargSword::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_VargSword::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CCorvusState_PS_VargSword::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
	
}

_bool CCorvusState_PS_VargSword::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.8f)
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

void CCorvusState_PS_VargSword::OnStateStart(const _float& In_fAnimationBlendTime)
{
	CPlayerStateBase::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 3);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_VargSword::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_VargSword::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_VargSword::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_VargSword::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_VargSword::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_VargSword::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_VargSword::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_VargSword::Call_AnimationEnd, this, placeholders::_1);
}
