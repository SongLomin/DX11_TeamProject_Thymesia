#include "stdafx.h"
#include "BossVarg/VargBossState_Attack1b.h"
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

GAMECLASS_C(CVargBossState_Attack1b);
CLONE_C(CVargBossState_Attack1b, CComponent)

void CVargBossState_Attack1b::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 61:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 68:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.5f, 1.5f, 9.f, 0.5f);
		break;
	case 88:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		break;
	}
}

HRESULT CVargBossState_Attack1b::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack1b::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = { -1.f, 0.f, 0.f };
	return S_OK;
}

void CVargBossState_Attack1b::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack2_1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack1b::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Attack1b::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack1b::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Attack1b::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack3a>().lock())
		m_bNextAttack = true;

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	for (auto& elem : pWeapons)
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.12f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_Attack1b::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CVargBossState_Attack1b::Call_OtherControllerHit, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: Attack1b -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_Attack1b::OnStateEnd()
{
	__super::OnStateEnd();
	m_bNextAttack = false;
	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_Attack1b::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CVargBossState_Attack1b::Call_OtherControllerHit, this, placeholders::_1);
}



void CVargBossState_Attack1b::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
}

void CVargBossState_Attack1b::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack1b::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Attack1b::Free() {}

_bool CVargBossState_Attack1b::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f && !m_bNextAttack)
	{
		int iRand(rand() % 2);

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack2b>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack2b2>(0.05f);
			break;
		}
		return true;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f && !m_bNextAttack)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack2b1>(0.05f);
		return true;
	}

	return false;
}

