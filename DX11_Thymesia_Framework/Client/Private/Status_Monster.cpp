#include "stdafx.h"
#include "Status_Monster.h"
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "Monster.h"


GAMECLASS_C(CStatus_Monster)
CLONE_C(CStatus_Monster, CComponent)

HRESULT CStatus_Monster::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CStatus_Monster::Initialize(void* pArg)
{
	if (pArg)
	{
		Init_Status(pArg);
	}


	return S_OK;
}

void CStatus_Monster::Start()
{
}

void CStatus_Monster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_tMonsterDesc.m_fHitedTime += fTimeDelta;
}

void CStatus_Monster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CStatus_Monster::Is_Dead()
{
	if (m_tMonsterDesc.m_fCurrentHP_Green <= DBL_EPSILON)
	{
		return true;
	}

	return false;
}

void CStatus_Monster::Init_Status(const void* pArg)
{
	if (pArg == nullptr)
		MSG_BOX("CStatus_Monster Error : Nullptr Arguments");

	CMonster::STATE_LINK_DESC tMonsterDesc;
	memcpy(&tMonsterDesc, pArg, sizeof(CMonster::STATE_LINK_DESC));

	MONSTERTYPE eMonsterType = tMonsterDesc.eMonType;


	switch (eMonsterType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_tMonsterDesc.m_fAtk = 55.f;
		m_tMonsterDesc.m_fMaxHP_white = 150.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_tMonsterDesc.m_fAtk = 45.f;
		m_tMonsterDesc.m_fMaxHP_white = 125.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		break;
	}

	m_tMonsterDesc.m_fCurrentHP_white = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fCurrentHP_Green = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fMaxHP_Green = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fHitedTime = 0.f;
	m_tMonsterDesc.m_fRecoveryAlramTime = 5.f;
	m_tMonsterDesc.m_fRecoveryTime = 7.f;
	m_tMonsterDesc.m_fHpBarDisableTime = 15.f;
	m_tMonsterDesc.m_iCueentParryCount = 0.f;
	m_tMonsterDesc.m_fCurrentParryingGauge = m_tMonsterDesc.m_fMaxParryingGauge;

}

void CStatus_Monster::Add_Damage(const _float& In_fDamage, ATTACK_OPTION eAttackOption)
{
	switch (eAttackOption)
	{
	case Client::ATTACK_OPTION::NONE:
		break;
	case Client::ATTACK_OPTION::NORMAL:
		Decrease_HP(m_tMonsterDesc.m_fCurrentHP_white, In_fDamage);
		Decrease_HP(m_tMonsterDesc.m_fCurrentHP_Green, In_fDamage * 0.2f);
		break;
	case Client::ATTACK_OPTION::PLAGUE:
		Decrease_HP(m_tMonsterDesc.m_fCurrentHP_Green, In_fDamage);
		break;
	case Client::ATTACK_OPTION::SPECIAL_ATTACK:
		break;
	case Client::ATTACK_OPTION::OPTION_END:
		break;
	default:
		break;
	}
}

void CStatus_Monster::Decrease_HP(_float& fCurrentHP, const _float& In_fDamage)
{
	fCurrentHP -= In_fDamage;
	if (fCurrentHP <= 0.f)
		fCurrentHP = 0.f;
}

void CStatus_Monster::Check_HitedTime()
{
	//가장 최근에 맞은 시간에 따라 처리되는 그게... 달라짐.
	//5
	if (m_tMonsterDesc.m_fHitedTime >= m_tMonsterDesc.m_fHpBarDisableTime)//15
	{
		//UI 꺼져야함

		return;
	}
	else if (m_tMonsterDesc.m_fHitedTime >= m_tMonsterDesc.m_fRecoveryTime)//7
	{
		//체력 회복 들어가고, UI에 체력 회복 깜빡임 끄기.
		

		return;
	}
	else if (m_tMonsterDesc.m_fHitedTime >= m_tMonsterDesc.m_fRecoveryAlramTime)//5
	{
		//체력 회복 알람

		return;
	}
}



