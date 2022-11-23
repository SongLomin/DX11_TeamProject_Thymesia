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

	Check_HitedTime(fTimeDelta);
}

void CStatus_Monster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CStatus_Monster::Restart()
{
	m_tMonsterDesc.m_fCurrentHP_white = m_tMonsterDesc.m_fMaxHP_white * 0.5f;
	m_tMonsterDesc.m_fCurrentHP_Green= m_tMonsterDesc.m_fCurrentHP_white;
	CallBack_ReStart();
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

	CMonster::STATE_LINK_MONSTER_DESC tMonsterDesc;
	memcpy(&tMonsterDesc, pArg, sizeof(CMonster::STATE_LINK_MONSTER_DESC));

	MONSTERTYPE eMonsterType = tMonsterDesc.eMonType;


	switch (eMonsterType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_tMonsterDesc.m_fAtk = 55.f;
		m_tMonsterDesc.m_fMaxHP_white = 150.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_AxeMan";
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_tMonsterDesc.m_fAtk = 45.f;
		m_tMonsterDesc.m_fMaxHP_white = 125.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_KnifeWoMan";
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		//TODO 공격력몰름 임시 
		m_tMonsterDesc.m_fAtk = 30.f;
		m_tMonsterDesc.m_fMaxHP_white = 125.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_Gardner";
		break;
	}

	m_tMonsterDesc.m_fCurrentHP_white = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fCurrentHP_Green = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fMaxHP_Green = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fHitedTime = 999.f;
	m_tMonsterDesc.m_fRecoveryAlramTime = 5.f;
	m_tMonsterDesc.m_fRecoveryTime = 7.f;
	m_tMonsterDesc.m_fHpBarDisableTime = 15.f;
	m_tMonsterDesc.m_iCueentParryCount = 0.f;
	m_tMonsterDesc.m_fCurrentParryingGauge = m_tMonsterDesc.m_fMaxParryingGauge;
	m_tMonsterDesc.m_fRecoveryAmountPercentageFromSecond = 0.3f;
	m_tMonsterDesc.m_fRecoveryMag = 1.f;
}

void CStatus_Monster::Add_Damage(const _float& In_fDamage, ATTACK_OPTION eAttackOption)
{

	switch (eAttackOption)
	{
	case Client::ATTACK_OPTION::NONE:
		break;
	case Client::ATTACK_OPTION::NORMAL:
		Decrease_White_HP(In_fDamage);
		Decrease_Green_HP(In_fDamage * 0.2f);
		break;
	case Client::ATTACK_OPTION::PLAGUE:
		Decrease_Green_HP(In_fDamage);
		break;
	case Client::ATTACK_OPTION::SPECIAL_ATTACK:
		break;
	case Client::ATTACK_OPTION::OPTION_END:
		break;
	default:
		break;
	}
}

void CStatus_Monster::Get_Desc(void* pOutDesc)
{
	memcpy(pOutDesc, &m_tMonsterDesc, sizeof(MONSTERDESC));
}

void CStatus_Monster::Decrease_White_HP(const _float& In_fDamage)
{
	Decrease_HP(m_tMonsterDesc.m_fCurrentHP_white, In_fDamage);

	m_tMonsterDesc.m_fHitedTime = 0.f;

	CallBack_Damged_White(m_tMonsterDesc.m_fCurrentHP_white /
		m_tMonsterDesc.m_fMaxHP_white);
}

void CStatus_Monster::Decrease_Green_HP(const _float& In_fDamage)
{
	Decrease_HP(m_tMonsterDesc.m_fCurrentHP_Green, In_fDamage);

	m_tMonsterDesc.m_fHitedTime = 0.f;

	if (m_tMonsterDesc.m_fCurrentHP_Green < m_tMonsterDesc.m_fCurrentHP_white)
		m_tMonsterDesc.m_fCurrentHP_Green = m_tMonsterDesc.m_fCurrentHP_white;

	CallBack_Damged_Green(m_tMonsterDesc.m_fCurrentHP_Green/
		m_tMonsterDesc.m_fMaxHP_Green);
}


void CStatus_Monster::Check_HitedTime(_float fTimeDelta)
{
	//가장 최근에 맞은 시간에 따라 처리되는 그게... 달라짐.
	//5
	if (m_tMonsterDesc.m_fHitedTime >= m_tMonsterDesc.m_fHpBarDisableTime)//15
	{
		//UI 꺼져야함
		CallBack_UI_Disable();
		return;
	}
	else if (m_tMonsterDesc.m_fHitedTime >= m_tMonsterDesc.m_fRecoveryTime)//8
	{
		//체력 회복 들어가고, UI에 체력 회복 깜빡임 끄기.
		CallBack_RecoeoryStart();

		m_tMonsterDesc.m_fCurrentHP_white = min
		(
			m_tMonsterDesc.m_fCurrentHP_white += ((m_tMonsterDesc.m_fMaxHP_white *
			m_tMonsterDesc.m_fRecoveryAmountPercentageFromSecond) * 
			m_tMonsterDesc.m_fRecoveryMag) * fTimeDelta,
			m_tMonsterDesc.m_fCurrentHP_Green) ;

		return;
	}
	else if (m_tMonsterDesc.m_fHitedTime >= m_tMonsterDesc.m_fRecoveryAlramTime)//5
	{
		//체력 회복 알람
		if (m_tMonsterDesc.m_fCurrentHP_Green >= 0.f)
			CallBack_RecoeoryAlram();
		
		return;
	}
}



