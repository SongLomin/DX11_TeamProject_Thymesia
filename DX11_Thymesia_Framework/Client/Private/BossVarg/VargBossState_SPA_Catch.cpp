#include "stdafx.h"
#include "BossVarg/VargBossState_SPA_Catch.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "UI_BloodOverlay.h"

GAMECLASS_C(CVargBossState_SPA_Catch);
CLONE_C(CVargBossState_SPA_Catch, CComponent)

void CVargBossState_SPA_Catch::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 221:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 228:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		break;
	}
}

HRESULT CVargBossState_SPA_Catch::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_Catch::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CVargBossState_SPA_Catch::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Catch");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_Catch::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_Catch::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Rotation_TargetToLookDir();
	Get_Owner().lock()->Get_Component<CVargBossState_SPA_Run>().lock()->Set_Count(0);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 225)
	{
		GAMEINSTANCE->Get_GameObjects<CUI_BloodOverlay>(LEVEL_STATIC).front().lock()->
			Call_Overlay(2.f);
	}
}

void CVargBossState_SPA_Catch::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CVargBossState_SPA_Catch::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pOwner.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_VARGTURNONSPOTLIGHT);

#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_Catch -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_SPA_Catch::OnStateEnd()
{
	__super::OnStateEnd();
}



void CVargBossState_SPA_Catch::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_SPA_Catch::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_Catch::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_Catch::Free()
{
}

_bool CVargBossState_SPA_Catch::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	return false;
}

