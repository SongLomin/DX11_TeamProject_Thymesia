#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_VargSwordStart.h"
#include "CorvusStates/CorvusState_PS_VargSword.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_VargSwordStart);
CLONE_C(CCorvusState_PS_VargSwordStart, CComponent)

void CCorvusState_PS_VargSwordStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_PS_VargSword>();
}

void CCorvusState_PS_VargSwordStart::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 37:
		TurnOn_Effect("Corvus_PW_VargSword_Weapon");
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_VargSwordStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_VargSwordStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_VargSwordStart::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_VargSword_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_VargSwordStart::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_VargSwordStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_VargSwordStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}



void CCorvusState_PS_VargSwordStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	CPlayerStateBase::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	Set_WeaponRender(false);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_VargSwordStart::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_VargSwordStart::Call_OtherControllerHit, this, placeholders::_1);

}

void CCorvusState_PS_VargSwordStart::OnStateEnd()
{
	CPlayerStateBase::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_VargSwordStart::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_VargSwordStart::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_VargSwordStart::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_VargSwordStart::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_VargSwordStart::Call_AnimationEnd, this, placeholders::_1);
}
