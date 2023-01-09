#include "stdafx.h"
#include "BossVarg/VargBossState_Attack2b2.h"
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
#include "PhysXCharacterController.h"
#include "MobWeapon.h"

GAMECLASS_C(CVargBossState_Attack2b2);
CLONE_C(CVargBossState_Attack2b2, CComponent)

void CVargBossState_Attack2b2::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 0:
		m_vShakingOffSet = { 0.f, 1.f, 0.f };
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.05f, 1.f, 9.f, 0.99f);
		break;
	case 52:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 58:
		m_vShakingOffSet = { 1.f, 1.f, 0.f };
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.5f, 1.f, 9.f, 0.7f);
		break;
	case 65:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		break;
	}
}

HRESULT CVargBossState_Attack2b2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack2b2::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CVargBossState_Attack2b2::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack2_2b");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack2b2::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Attack2b2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack2b2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Attack2b2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	for (auto& elem : pWeapons)
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_Attack2b2::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CVargBossState_Attack2b2::Call_OtherControllerHit, this, placeholders::_1);
#ifdef _DEBUG_COUT_
	cout << "VargState: Attack2b2 -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_Attack2b2::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_Attack2b2::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CVargBossState_Attack2b2::Call_OtherControllerHit, this, placeholders::_1);
}

void CVargBossState_Attack2b2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_Attack2b2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack2b2::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Attack2b2::Free()
{
}

_bool CVargBossState_Attack2b2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	return false;
}

