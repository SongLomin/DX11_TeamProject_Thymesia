#include "stdafx.h"
#include "BossVarg/VargBossState_AvoidAttack.h"
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


GAMECLASS_C(CVargBossState_AvoidAttack);
CLONE_C(CVargBossState_AvoidAttack, CComponent)

void CVargBossState_AvoidAttack::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 63:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.3f, 1.f, 9.f, 0.25f);
		break;
	case 66:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.6f, 1.f, 9.f, 0.5f);
		break;
	}
}

HRESULT CVargBossState_AvoidAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_AvoidAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = { 0.f, -1.f, 0.f };
	return S_OK;
}

void CVargBossState_AvoidAttack::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_AvoidAttack1");

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_AvoidAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_AvoidAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_AvoidAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_AvoidAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::DOWN_HIT, 1.f);
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_AvoidAttack::Call_NextKeyFrame, this, placeholders::_1);

	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CVargBossState_AvoidAttack::Call_OtherControllerHit, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: AvoidAttack -> OnStateStart" << endl;
#endif
}



void CVargBossState_AvoidAttack::OnStateEnd()
{
	__super::OnStateEnd();

	m_bAttackLookAtLimit = false;

	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_AvoidAttack::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -=
		bind(&CVargBossState_AvoidAttack::Call_OtherControllerHit, this, placeholders::_1);
}




void CVargBossState_AvoidAttack::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_AvoidAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_AvoidAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_AvoidAttack::Free()
{

}

_bool CVargBossState_AvoidAttack::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f)
	{
		m_bAttackLookAtLimit = false;
	}

	return false;
}

