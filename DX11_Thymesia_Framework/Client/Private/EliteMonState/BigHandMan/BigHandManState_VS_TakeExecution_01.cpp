#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"
#include "Status_Monster.h"
#include "UI_Utils.h"
#include "MobWeapon.h"
#include "Interaction_Item.h"
#include "MonsterHPBar_Base.h"


GAMECLASS_C(CBigHandManState_VS_TakeExecution_01);
CLONE_C(CBigHandManState_VS_TakeExecution_01, CComponent)

HRESULT CBigHandManState_VS_TakeExecution_01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_VS_TakeExecution_01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_VS_TakeExecution_01::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_VS_TakeExecution_01");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_VS_TakeExecution_01::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_VS_TakeExecution_01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();



	if (!m_bAnimationStop)
	{
		pCurrentPlayer.lock()->Get_Transform()->LookAt2D(Get_OwnerMonster()->Get_Transform()->Get_Position());

		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}
	else
	{
		Get_OwnerMonster()->Set_PassIndex(7);
		m_fDissolveTime -= fTimeDelta;

		_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
		Get_OwnerMonster()->Set_DissolveAmount(fDissolveAmount);
	}
	
}


void CBigHandManState_VS_TakeExecution_01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

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



void CBigHandManState_VS_TakeExecution_01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Get_OwnerMonster()->Release_Monster();

	Rotation_TargetToLookDir();

	Get_OwnerMonster()->Set_EliteExecutionStartOnOff(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_fDissolveTime = 4.f;
	m_bAnimationStop = false;

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_VS_TakeExecution_01::OnStateEnd()
{
	__super::OnStateEnd();

	weak_ptr<CInteraction_Item> pItem = GAMEINSTANCE->Add_GameObject<CInteraction_Item>(m_CreatedLevel);
	pItem.lock()->Get_Transform()->Set_Position(m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 0.5f, 0.f, 0.f));
	pItem.lock()->Add_Item(CUI_Utils::ConvertMonsterTypeToSkillPiece(m_eMonType));
	pItem.lock()->Add_Item(ITEM_NAME::MEMORY02);

	Weak_StaticCast<CMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(false);


}

void CBigHandManState_VS_TakeExecution_01::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
}

void CBigHandManState_VS_TakeExecution_01::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_VS_TakeExecution_01::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_VS_TakeExecution_01::Free()
{

}

_bool CBigHandManState_VS_TakeExecution_01::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;
	//58보다크면 흠


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 58)
	{
		m_bAnimationStop = true;
		m_pOwner.lock()->Get_Component<CStatus_Monster>().lock()->CallBack_UI_Disable();
		
	}

	return false;
}

