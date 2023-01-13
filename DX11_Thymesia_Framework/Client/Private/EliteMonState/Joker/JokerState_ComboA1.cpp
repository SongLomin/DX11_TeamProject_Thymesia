#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_ComboA1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"


GAMECLASS_C(CJokerState_ComboA1);
CLONE_C(CJokerState_ComboA1, CComponent)

HRESULT CJokerState_ComboA1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_ComboA1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_ComboA1::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_ComboA01");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_ComboA1::Call_AnimationEnd, this, placeholders::_1);*/
}

void CJokerState_ComboA1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_ComboA1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_ComboA1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.3f);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_ComboA1::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_ComboA1::Free()
{

}

_bool CJokerState_ComboA1::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		Get_OwnerCharacter().lock()->Change_State<CJokerState_ComboA2>(0.05f);
		return true;
	}
	
	return false;
}



	

