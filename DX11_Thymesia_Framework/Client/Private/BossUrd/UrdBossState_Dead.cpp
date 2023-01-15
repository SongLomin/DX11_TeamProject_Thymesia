#include "stdafx.h"
#include "BossUrd/UrdBossState_Dead.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "JavelinWeapon.h"
#include "UrdWeapon.h"
#include "UI_ScriptQueue.h"
#include "UI_Landing.h"

GAMECLASS_C(CUrdBossState_Dead);
CLONE_C(CUrdBossState_Dead, CComponent)

HRESULT CUrdBossState_Dead::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Dead::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Dead::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Dead|BaseLayer");

	m_fDissolveTime = 4.f;

	m_bAnimEnd = false;

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Dead::Call_AnimationEnd, this, placeholders::_1);


}

void CUrdBossState_Dead::Tick(_float fTimeDelta)
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


void CUrdBossState_Dead::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Dead::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CUrdBossState_Dead::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bOnce = true;

	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,4);
	
	GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_KILL_BOSS, 2.f);


	
	GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL::LEVEL_STATIC).front().lock()->Call_SetScript_Urd_Dead();

	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Urd_WeaponShine_Phase2", GET_SINGLE(CGameManager)->Get_StoredEffectIndex("Urd_WeaponShine_Phase2"));
}

void CUrdBossState_Dead::OnStateEnd()
{
	__super::OnStateEnd();

	m_bAnimEnd = true;

	Get_OwnerMonster()->Release_Monster();

}

void CUrdBossState_Dead::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	//Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Dead::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Dead::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Dead::Free()
{

}

_bool CUrdBossState_Dead::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	weak_ptr<CUrd> pUrd = Weak_StaticCast<CUrd>(pMonster).lock();
	list<weak_ptr<CJavelinWeapon>> pJavelinWeapons = pUrd.lock()->Get_JavelinWeapons();
	list<weak_ptr<CMobWeapon>>	pDecoWeapons = pUrd.lock()->Get_DecoWeapons();

	//한 0.1 이상에서부터 싹다 없애야될듯?

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.1f && m_bOnce)
	{
		m_bOnce = false;

		for (auto& elnm : pJavelinWeapons)
		{
			elnm.lock()->Set_RenderOnOff(false);
		}
		for (auto& elnm : pDecoWeapons)
		{
			elnm.lock()->Set_RenderOnOff(false);
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.9f)
	{
		m_bAnimEnd = true;
	}

	return false;
}

