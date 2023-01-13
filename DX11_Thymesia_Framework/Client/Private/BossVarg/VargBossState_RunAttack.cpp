#include "stdafx.h"
#include "BossVarg/VargBossState_RunAttack.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "../Public/BossVarg/Varg.h"
#include "VargWeapon.h"
#include "MobWeapon.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_RunAttack);
CLONE_C(CVargBossState_RunAttack, CComponent)

void CVargBossState_RunAttack::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 20:
		m_vShakingOffSet = { 0.f, 1.f, 0.f };
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.05f, 1.f, 9.f, 0.99f);
		break;
	case 52:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 54:
		m_vShakingOffSet = { -1.f, 1.f, 0.f };
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.6f, 1.f, 9.f, 0.7f);
		break;
	case 59:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		break;
	}
}

HRESULT CVargBossState_RunAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_RunAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CVargBossState_RunAttack::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_RaidAttack1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_RunAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_RunAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CVargBossState_RunAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Rotation_TargetToLookDir();
	Check_AndChangeNextState();
}

void CVargBossState_RunAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	for (auto& elem : pWeapons)
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_RunAttack::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CVargBossState_RunAttack::Call_OtherControllerHit, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: RunAttack -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_RunAttack::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_RunAttack::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CVargBossState_RunAttack::Call_OtherControllerHit, this, placeholders::_1);
}



void CVargBossState_RunAttack::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_RunAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_RunAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_RunAttack::Free() { }

_bool CVargBossState_RunAttack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

