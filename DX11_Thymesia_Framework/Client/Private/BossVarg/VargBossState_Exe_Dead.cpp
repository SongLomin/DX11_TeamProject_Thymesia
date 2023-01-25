#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_Dead.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

#include "UI_Landing.h"
#include "Inventory.h"
#include "UIManager.h"

GAMECLASS_C(CVargBossState_Exe_Dead);
CLONE_C(CVargBossState_Exe_Dead, CComponent)

HRESULT CVargBossState_Exe_Dead::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Exe_Dead::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Exe_Dead::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_Dead");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Exe_Dead::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_Dead::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Exe_Dead::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Exe_Dead::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CVargBossState_Exe_Dead::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	if (In_KeyIndex == 10)
	{
		m_pOwner.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_VARGTURNOFFSPOTLIGHT);
		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->OnEventMessage((_uint)EVENT_TYPE::ON_VARGTURNOFFSPOTLIGHT);
	}
}

void CVargBossState_Exe_Dead::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CVargBossState_Exe_Dead::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_Dead -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Exe_Dead::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CVargBossState_Exe_Dead::Call_NextKeyFrame, this, placeholders::_1);

}

void CVargBossState_Exe_Dead::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_KILL_BOSS);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::VARG_KEY);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::SKILLPIECE_VARGSWORD);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::MEMORY01);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::MEMORY01);
	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()
		->Push_Item(ITEM_NAME::MEMORY02);

	GAMEINSTANCE->PlayBGM("BGM_STAGE_3.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_End>(0.05f);

	GET_SINGLE(CGameManager)->Activate_Section(100, EVENT_TYPE::ON_UNLOCK_SECTION);
}

void CVargBossState_Exe_Dead::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Exe_Dead::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_Dead::Free()
{

}

_bool CVargBossState_Exe_Dead::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

