#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Execution_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "GameManager.h"
#include "Inventory.h"
#include "Interaction_Item.h"
#include "UI_Utils.h"
#include "MobWeapon.h"
#include "MonsterHPBar_Base.h"

GAMECLASS_C(CJokerState_Execution_Loop);
CLONE_C(CJokerState_Execution_Loop, CComponent)

HRESULT CJokerState_Execution_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Execution_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Execution_Loop::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_TakeExecution_Loop");

}

void CJokerState_Execution_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	Get_OwnerMonster()->Set_PassIndex(7);
	m_fDissolveTime -= fTimeDelta;

	_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
	Get_OwnerMonster()->Set_DissolveAmount(fDissolveAmount);
}


void CJokerState_Execution_Loop::LateTick(_float fTimeDelta)
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

void CJokerState_Execution_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();

	Get_OwnerMonster()->Release_Monster();

	m_fDissolveTime = 4.f;

	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()->Push_Item(ITEM_NAME::GARDEN_KEY);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()->Push_Item(ITEM_NAME::MEMORY01);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()->Push_Item(ITEM_NAME::MEMORY01);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()->Push_Item(ITEM_NAME::MEMORY01);

	weak_ptr<CInteraction_Item> pItem = GAMEINSTANCE->Add_GameObject<CInteraction_Item>(m_CreatedLevel);
	pItem.lock()->Get_Transform()->Set_Position(m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 0.5f, 0.f, 0.f));
	pItem.lock()->Add_Item(CUI_Utils::ConvertMonsterTypeToSkillPiece(m_eMonType));
	pItem.lock()->Add_Item(ITEM_NAME::MEMORY02);

	Weak_StaticCast<CMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(false);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_Execution_Loop::OnStateEnd()
{
	__super::OnStateEnd();


}


void CJokerState_Execution_Loop::Free()
{

}

_bool CJokerState_Execution_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

void CJokerState_Execution_Loop::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CEliteStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

