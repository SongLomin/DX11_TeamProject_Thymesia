#include "stdafx.h"
#include "BossVarg/VargBossState_IdleGeneral.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "BossMonster.h"
#include "MonsterHPBar_Boss.h"
#include "GameManager.h"
#include "UI_FadeMask.h"

GAMECLASS_C(CVargBossState_IdleGeneral);
CLONE_C(CVargBossState_IdleGeneral, CComponent)

HRESULT CVargBossState_IdleGeneral::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_IdleGeneral::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CVargBossState_IdleGeneral::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_IdleGeneral");
}

void CVargBossState_IdleGeneral::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_IdleGeneral::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	CBossStateBase::Rotation_TargetToLookDir();

	if (CallBack_EventTrigger.empty())
	{
		Check_AndChangeNextState();
	}
	else
	{
		_bool bEnd = false;

		CallBack_EventTrigger(fTimeDelta, bEnd);

		if (bEnd)
			CallBack_EventTrigger.Clear();
	}

}



void CVargBossState_IdleGeneral::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bFadeOutTrigger = true;
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	Weak_StaticCast<CBossMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(false);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: RunStart -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
#endif// _DEBUG

	
}

void CVargBossState_IdleGeneral::OnStateEnd()
{
	__super::OnStateEnd();
	
}

void CVargBossState_IdleGeneral::EventTrigger(_float fTimeDelta, _bool& Out_bEnd)
{
	m_fAccTime += fTimeDelta;

	if (m_bFadeOutTrigger && 3.f <= m_fAccTime)
	{
		m_bFadeOutTrigger = false;
		weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
		pFadeMask.lock()->Set_Fade_Delay(0.f, 1.f, 2.f, 1.f, EASING_TYPE::LINEAR);
	}

	if (6.f <= m_fAccTime)
	{
		CStateBase::Get_OwnerCharacter().lock()->Change_State<CVargBossState_Start>(0.05f);
		
		m_fAccTime = 0.f;
		Out_bEnd   = true;
	}
}

void CVargBossState_IdleGeneral::Free()
{
}

_bool CVargBossState_IdleGeneral::Check_AndChangeNextState()
{
	if (!CBossStateBase::Check_Requirement())
		return false;

	_float fPToMDistance(CBossStateBase::Get_DistanceWithPlayer()); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 18.f)
	{
		//CStateBase::Get_OwnerCharacter().lock()->Change_State<CVargBossState_Start>(0.05f);
		GET_SINGLE(CGameManager)->Activate_Section(100, EVENT_TYPE::ON_EXIT_SECTION);
		GET_SINGLE(CGameManager)->Activate_Section(100, EVENT_TYPE::ON_LOCK_SECTION);
		GET_SINGLE(CGameManager)->Activate_SectionLight(0, EVENT_TYPE::ON_ENTER_SECTION);
		GET_SINGLE(CGameManager)->Activate_Fog(1);

		CallBack_EventTrigger += bind(&CVargBossState_IdleGeneral::EventTrigger, this, placeholders::_1, placeholders::_2);

		return true;
	}

	return false;
}

