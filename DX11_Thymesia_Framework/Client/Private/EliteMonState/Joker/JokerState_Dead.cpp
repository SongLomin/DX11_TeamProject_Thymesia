#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Dead.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"
#include "MonsterHPBar_Elite.h"
#include "Inventory.h"
#include "UI_Utils.h"
#include "Interaction_Item.h"


GAMECLASS_C(CJokerState_Dead);
CLONE_C(CJokerState_Dead, CComponent)

HRESULT CJokerState_Dead::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Dead::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Dead::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_Dead");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Dead::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_Dead::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if(!m_bAnimEnd)
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	else
	{
		Get_OwnerMonster()->Set_PassIndex(7);
		m_fDissolveTime -= fTimeDelta;
	
		_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
		Get_OwnerMonster()->Set_DissolveAmount(fDissolveAmount);
	}
}


void CJokerState_Dead::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//Monster Die
	if (m_fDissolveTime < 0.f)
	{
		m_pOwner.lock()->Set_Enable(false);
		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
		for (auto& elem : pWeapons)
			elem.lock()->Set_Enable(false);

		Get_OwnerMonster()->Set_PassIndex(0);
	}

	Check_AndChangeNextState();
}



void CJokerState_Dead::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_fDissolveTime = 4.f;

	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Joker_Passive", GET_SINGLE(CGameManager)->Get_StoredEffectIndex("Joker_Passive"));

#ifdef _DEBUG_COUT_
	cout << "JokerState: Idle -> OnStateStart" << endl;
#endif // _DEBUG

}

void CJokerState_Dead::OnStateEnd()
{
	__super::OnStateEnd();
}

void CJokerState_Dead::Call_AnimationEnd(_uint iEndAnimIndex)
{
}

void CJokerState_Dead::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_Dead::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_Dead::Free()
{

}

_bool CJokerState_Dead::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

