#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_CaneSword_Start.h"
#include "CorvusStates/CorvusState_PS_CaneSword.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Weapon.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_CaneSword_Start);
CLONE_C(CCorvusState_PS_CaneSword_Start, CComponent)

void CCorvusState_PS_CaneSword_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_PS_CaneSword>();
}

void CCorvusState_PS_CaneSword_Start::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
}

HRESULT CCorvusState_PS_CaneSword_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_CaneSword_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_CaneSword_Start::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_CaneSword_Fast");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_CaneSword_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_CaneSword_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_CaneSword_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}



void CCorvusState_PS_CaneSword_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	CPlayerStateBase::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	Set_WeaponRender(false);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_CaneSword_Start::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_CaneSword_Start::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_CaneSword_Start::OnStateEnd()
{
	CPlayerStateBase::OnStateEnd();

	TurnOn_Effect("Corvus_PW_EyeGlow_Special");
	TurnOn_Effect("Corvus_PW_CaneSword_SP02_Weapon");

	GET_SINGLE(CGameManager)->Activate_Zoom(3.f, 2.f, EASING_TYPE::QUINT_IN);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_CaneSword_Start::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_CaneSword_Start::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_CaneSword_Start::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_CaneSword_Start::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_CaneSword_Start::Call_AnimationEnd, this, placeholders::_1);
}
