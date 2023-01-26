#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_BigHand.h"
#include "CorvusStates/CorvusState_PS_BigHand_End.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Effect_Decal.h"
#include "CorvusStates/CorvusStates.h"


GAMECLASS_C(CCorvusState_PS_BigHand);
CLONE_C(CCorvusState_PS_BigHand, CComponent)

void CCorvusState_PS_BigHand::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_PS_BigHand_End>();
}

void CCorvusState_PS_BigHand::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 60:
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	case 103:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.2f);

		XMStoreFloat4x4(&m_DecalDesc.WorldMatrix, OwnerWorldMatrix);

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);
	}
	return;
	case 130:
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_BigHand::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_BigHand::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_BigHand::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_BigHand_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_BigHand::Call_AnimationEnd, this, placeholders::_1);

	m_DecalDesc.vScale = { 15.f, 15.f, 0.1f };
	m_DecalDesc.vPosition = {0.1f, 0.01f, 0.5f, 1.f};
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 2.f;
	m_DecalDesc.vColor = _float3(0.f, 1.f, 0.7f);
	m_DecalDesc.strTextureTag = "DecalTexture";
}

void CCorvusState_PS_BigHand::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_BigHand::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}



void CCorvusState_PS_BigHand::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_BigHand::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_BigHand::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BigHand::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_BigHand::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_BigHand::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BigHand::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_BigHand::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_BigHand::Call_AnimationEnd, this, placeholders::_1);
}
