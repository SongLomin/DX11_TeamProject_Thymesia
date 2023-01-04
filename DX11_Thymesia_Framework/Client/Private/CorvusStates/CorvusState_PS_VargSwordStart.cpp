#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_VargSwordStart.h"
#include "CorvusStates/CorvusState_PS_VargSword.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_VargSwordStart);
CLONE_C(CCorvusState_PS_VargSwordStart, CComponent)

void CCorvusState_PS_VargSwordStart::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_PS_VargSword>();
}

void CCorvusState_PS_VargSwordStart::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
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
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_VargSwordStart::Call_AnimationEnd, this);
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
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_VargSwordStart::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_VargSwordStart::Call_OtherControllerHit, this, placeholders::_1);

}

void CCorvusState_PS_VargSwordStart::OnStateEnd()
{
	__super::OnStateEnd();

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
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_VargSwordStart::Call_AnimationEnd, this);
}
