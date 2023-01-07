#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_BatRoar.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_BatRoar);
CLONE_C(CCorvusState_PS_BatRoar, CComponent)

void CCorvusState_PS_BatRoar::Call_AnimationEnd()
{
	__super::Call_AnimationEnd();
}

void CCorvusState_PS_BatRoar::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	}
}

HRESULT CCorvusState_PS_BatRoar::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_BatRoar::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_BatRoar::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Bat_Roar_A");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_BatRoar::Call_AnimationEnd, this);
}

void CCorvusState_PS_BatRoar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_BatRoar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCorvusState_PS_BatRoar::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_BatRoar::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_BatRoar::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_BatRoar::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_BatRoar::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_BatRoar::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_BatRoar::Call_AnimationEnd, this);
}
