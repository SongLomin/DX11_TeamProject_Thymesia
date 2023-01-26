#include "stdafx.h"
#include "CorvusStates/CorvusState_PS.h"
#include "CorvusStates/CorvusState_Idle.h"
#include "Collider.h"
#include "Status_Player.h"
#include "Attack_Area.h"
#include "Status_Monster.h"
#include "GameManager.h"
#include "Weapon.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS);

void CCorvusState_PS::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	if (m_iAnimIndex != iEndAnimIndex)
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
}

HRESULT CCorvusState_PS::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
}

void CCorvusState_PS::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_PS::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CCorvusState_PS::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	Set_WeaponRender(false);
}

void CCorvusState_PS::OnStateEnd()
{
	__super::OnStateEnd();
	Set_WeaponRender(true);
}

void CCorvusState_PS::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_PS::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::MONSTER_ATTACK)
	{
		weak_ptr<CStatus_Player> pStatus = Weak_StaticCast<CStatus_Player>(m_pStatusCom);

		weak_ptr<CAttackArea>		pAttackArea				= Weak_StaticCast<CAttackArea>(pOtherCollider.lock()->Get_Owner());
		weak_ptr<CCharacter>		pMonsterFromCharacter	= pAttackArea.lock()->Get_ParentObject();
		weak_ptr<CStatus_Monster>	pMonsterStatusCom		= Weak_StaticCast< CStatus_Monster>(pMonsterFromCharacter.lock()->Get_Status());

		pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
		_bool bGroggy(pMonsterStatusCom.lock()->Is_Groggy());

		if (bGroggy)
		{
			pMonsterFromCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_GROGGY);
		}
	}
}

_bool CCorvusState_PS::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

void CCorvusState_PS::Set_WeaponRender(const _bool bRender)
{
	list<weak_ptr<CWeapon>>	pWeapons = m_pOwnerFromPlayer.lock()->Get_Weapon();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_RenderOnOff(bRender);
	}
}

void CCorvusState_PS::TurnOn_Effect(const std::string& szPlagueWeaponName)
{
	GET_SINGLE(CGameManager)->Store_EffectIndex(szPlagueWeaponName.c_str(), GET_SINGLE(CGameManager)->Use_EffectGroup(szPlagueWeaponName, m_pTransformCom, _uint(TIMESCALE_LAYER::PLAYER)));
}

void CCorvusState_PS::TurnOff_Effect(const std::string& szPlagueWeaponName)
{
	GET_SINGLE(CGameManager)->UnUse_EffectGroup(szPlagueWeaponName, GET_SINGLE(CGameManager)->Get_StoredEffectIndex(szPlagueWeaponName.c_str()));
}

void CCorvusState_PS::Free()
{
}
