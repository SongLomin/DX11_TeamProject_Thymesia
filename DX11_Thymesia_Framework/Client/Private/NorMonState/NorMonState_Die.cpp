#include "stdafx.h"
#include "NorMonState/NorMonState_Die.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Player.h"
#include "Monster.h"
#include "MobWeapon.h"
#include "Status_Monster.h"
#include "Status_Player.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"

GAMECLASS_C(CNorMonState_Die);
CLONE_C(CNorMonState_Die, CComponent)

void CNorMonState_Die::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 1:
		m_vShakingOffSet = { 0.f, 1.f, 0.f };
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.5f, 1.f, 9.f, 0.25f);
		break;
	}
}

HRESULT CNorMonState_Die::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Die::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CNorMonState_Die::Start()
{
	__super::Start();

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}


	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Die01|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Die01");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_Dead");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_Dead");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Die01|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::BALLOON:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Die");
		break;
	}


	m_fDissolveTime = 4.f;

	m_bAnimEnd = false;
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Die::Call_AnimationEnd, this);
}

void CNorMonState_Die::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bAnimEnd)
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	else
	{
		Get_OwnerMonster()->Set_PassIndex(7);
		m_fDissolveTime -= fTimeDelta;

		_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
		Get_OwnerMonster()->Set_DissolveAmount(fDissolveAmount);		
	}
}

void CNorMonState_Die::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_fDissolveTime < 0.f)
	{
		m_pOwner.lock()->Set_Enable(false);
		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();
		for (auto& elem : pWeapons)
			elem.lock()->Set_Enable(false);
	}

	Check_AndChangeNextState();
}

void CNorMonState_Die::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CNorMonState_Die::Call_NextKeyFrame, this, placeholders::_1);


	m_pOwner.lock()->Get_ComponentByType<CStatus_Monster>().lock()->CallBack_UI_Disable();

	GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);

	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();

	Get_OwnerMonster()->Release_Monster();
	
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::BASIL);

	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::MEMORY01);

#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif // _DEBUG_COUT_
}

void CNorMonState_Die::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CNorMonState_Die::Call_NextKeyFrame, this, placeholders::_1);
}

void CNorMonState_Die::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Die::Call_AnimationEnd, this);
}


void CNorMonState_Die::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	m_bAnimEnd = true;




	//Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}


_bool CNorMonState_Die::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}


void CNorMonState_Die::Free()
{

}