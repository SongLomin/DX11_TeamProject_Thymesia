#include "stdafx.h"
#include "Status_Monster.h"
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "Monster.h"


GAMECLASS_C(CStatus_Monster)
CLONE_C(CStatus_Monster, CComponent)


HRESULT CStatus_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStatus_Monster::Initialize(void* pArg)
{
	if (pArg)
	{
		Init_Status(pArg);
	}
	Full_Recovery();
	return S_OK;
}

void CStatus_Monster::Start()
{
}

void CStatus_Monster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_tMonsterDesc.m_fHitedTime += fTimeDelta;

	Update_HitedTime(fTimeDelta);
	Update_ParryRecoveryTime(fTimeDelta);

}

void CStatus_Monster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CStatus_Monster::Restart()
{
	m_tMonsterDesc.m_fCurrentHP_white = m_tMonsterDesc.m_fMaxHP_white * 0.5f;
	m_tMonsterDesc.m_fCurrentHP_Green= m_tMonsterDesc.m_fCurrentHP_white;
	m_tMonsterDesc.m_fCurrentParryingGauge = 0.f;

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
	m_eMonsterType = eMonsterType;
	Init_StatusFromMonsterType(eMonsterType);
	
}

void CStatus_Monster::Add_Damage(const _float In_fDamage, ATTACK_OPTION eAttackOption)
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
		Decrease_White_HP(In_fDamage);
		Decrease_Green_HP(In_fDamage * 0.2f);
		break;
	default:
		break;
	}
}

void CStatus_Monster::Full_Recovery()
{
	Init_StatusFromMonsterType(m_eMonsterType);
	Callback_Full_Recovery();
}

void CStatus_Monster::Get_Desc(void* pOutDesc)
{
	MONSTERDESC* pMonsterDesc = (MONSTERDESC*)pOutDesc;

	*pMonsterDesc = m_tMonsterDesc;

//	memcpy(pOutDesc, &m_tMonsterDesc, sizeof(MONSTERDESC));
}

void CStatus_Monster::Decrease_White_HP(const _float In_fDamage)
{
	Decrease_HP(m_tMonsterDesc.m_fCurrentHP_white, In_fDamage);

	m_tMonsterDesc.m_fHitedTime = 0.f;

	_float fParryGaugeRatio = m_tMonsterDesc.m_fCurrentParryingGauge / m_tMonsterDesc.m_fMaxParryingGauge;
	Set_ParryRecoveryTime(fParryGaugeRatio);


	CallBack_Damged_White(m_tMonsterDesc.m_fCurrentHP_white /
		m_tMonsterDesc.m_fMaxHP_white);
}

void CStatus_Monster::Decrease_Green_HP(const _float In_fDamage)
{
	Decrease_HP(m_tMonsterDesc.m_fCurrentHP_Green, In_fDamage);

	m_tMonsterDesc.m_fHitedTime = 0.f;

	if (m_tMonsterDesc.m_fCurrentHP_Green < m_tMonsterDesc.m_fCurrentHP_white)
		m_tMonsterDesc.m_fCurrentHP_Green = m_tMonsterDesc.m_fCurrentHP_white;

	_float fParryGaugeRatio = m_tMonsterDesc.m_fCurrentParryingGauge / m_tMonsterDesc.m_fMaxParryingGauge;
	Set_ParryRecoveryTime(fParryGaugeRatio);


	CallBack_Damged_Green(m_tMonsterDesc.m_fCurrentHP_Green/
		m_tMonsterDesc.m_fMaxHP_Green);
}

void CStatus_Monster::Add_ParryGauge(const _float In_fDamage)
{
	Increase_HP(m_tMonsterDesc.m_fCurrentParryingGauge,
				m_tMonsterDesc.m_fMaxParryingGauge,
				In_fDamage);

	_float fParryGaugeRatio = m_tMonsterDesc.m_fCurrentParryingGauge / m_tMonsterDesc.m_fMaxParryingGauge;
	CallBack_UpdateParryGauge(fParryGaugeRatio, true);
	Set_ParryRecoveryTime(fParryGaugeRatio);
	m_tMonsterDesc.m_fHitedTime = 0.f;

	//패링 게이지가 꽉 찼다면 체력 다 0으로 만들기
	if (fParryGaugeRatio >= 1.f)
	{
		m_tMonsterDesc.m_fCurrentHP_Green = 0.f;
		m_tMonsterDesc.m_fCurrentHP_white= 0.f;

		CallBack_Damged_White(0.f);
		CallBack_Damged_Green(0.f);
	}


}

_bool CStatus_Monster::Is_Groggy() const
{
	_float fParryGaugeRatio = m_tMonsterDesc.m_fCurrentParryingGauge / m_tMonsterDesc.m_fMaxParryingGauge;

	return fParryGaugeRatio >= 1.f - DBL_EPSILON;
}

void CStatus_Monster::Heal(_float fHealAmount)
{
	m_tMonsterDesc.m_fCurrentHP_white += fHealAmount;

	if (m_tMonsterDesc.m_fCurrentHP_white > m_tMonsterDesc.m_fMaxHP_white)
	{
		m_tMonsterDesc.m_fCurrentHP_white = m_tMonsterDesc.m_fMaxHP_white;
	}
	m_tMonsterDesc.m_fCurrentHP_Green = m_tMonsterDesc.m_fCurrentHP_white;

	Callback_UpdateHP(m_tMonsterDesc);
}

void CStatus_Monster::Set_ParryRecoveryTime(const _float fRatio)
{
	if (fRatio > 0.8f)
		m_tMonsterDesc.m_fParryGaugeRecoveryTime = 10.f;
	else if (fRatio > 0.4f)
		m_tMonsterDesc.m_fParryGaugeRecoveryTime = 8.f;
	else
		m_tMonsterDesc.m_fParryGaugeRecoveryTime = 6.f;
}


void CStatus_Monster::Update_HitedTime(_float fTimeDelta)
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
		if (m_tMonsterDesc.m_fCurrentHP_Green > 0.f &&
			(m_tMonsterDesc.m_fCurrentHP_white < m_tMonsterDesc.m_fCurrentHP_Green))
			CallBack_RecoeoryAlram();

		return;
	}
}

void CStatus_Monster::Update_ParryRecoveryTime(_float fTimeDelta)
{
	if (m_tMonsterDesc.m_fCurrentHP_Green <= 0.f)
		return;

	if (m_tMonsterDesc.m_fCurrentParryingGauge <= 0.f)
		return;
	m_tMonsterDesc.m_fParryGaugeRecoveryTime -= fTimeDelta;

	//패리 게이지가 남아있다? 아직 회복까지 남은 시간이 있다.
	if (m_tMonsterDesc.m_fParryGaugeRecoveryTime > 0.f)
		return;

	//초당 최대 패링게이지의 2할만큼 빠짐.
	m_tMonsterDesc.m_fCurrentParryingGauge -= (m_tMonsterDesc.m_fMaxParryingGauge * 0.2f) * fTimeDelta;
	if (m_tMonsterDesc.m_fCurrentParryingGauge <= 0.f)
		m_tMonsterDesc.m_fCurrentParryingGauge = 0.f;

	CallBack_UpdateParryGauge(m_tMonsterDesc.m_fCurrentParryingGauge / m_tMonsterDesc.m_fMaxParryingGauge,
		false);

}

void CStatus_Monster::Init_StatusFromMonsterType(MONSTERTYPE eMonsterType)
{
	switch (eMonsterType)
	{
	case Client::MONSTERTYPE::BALLOON:
		m_tMonsterDesc.m_fAtk = 10.f;
		m_tMonsterDesc.m_fMaxHP_white = 100.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 1.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 1;
		m_tMonsterDesc.m_szModelKey = "Balloon";
		break;
	case Client::MONSTERTYPE::AXEMAN:
		m_tMonsterDesc.m_fAtk = 20.f;
		m_tMonsterDesc.m_fMaxHP_white = 150.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 100.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_AxeMan";
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_tMonsterDesc.m_fAtk = 25.f;
		m_tMonsterDesc.m_fMaxHP_white = 125.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 100.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_KnifeWoman";
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		//TODO 공격력몰름 임시
		m_tMonsterDesc.m_fAtk = 20.f;
		m_tMonsterDesc.m_fMaxHP_white = 175.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 100.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_Gardner";
		break;	
	case Client::MONSTERTYPE::VARG:
		//TODO 공격력몰름 임시
		m_tMonsterDesc.m_fAtk = 50.f;
		m_tMonsterDesc.m_fMaxHP_white = 3000.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 1000.f;
		m_tMonsterDesc.m_iLifeCount = 2;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Boss_Varg";
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_tMonsterDesc.m_fAtk = 30.f;
		m_tMonsterDesc.m_fMaxHP_white = 250.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_Gardner";
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_tMonsterDesc.m_fAtk = 25.f;
		m_tMonsterDesc.m_fMaxHP_white = 300.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Mon_AxeMan";
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN :
		m_tMonsterDesc.m_fAtk = 20.f;
		m_tMonsterDesc.m_fMaxHP_white = 150.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Skullman";
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_tMonsterDesc.m_fAtk = 30.f;
		m_tMonsterDesc.m_fMaxHP_white = 350.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 400.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Armorman";
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_tMonsterDesc.m_fAtk = 25.f;
		m_tMonsterDesc.m_fMaxHP_white = 250.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 300.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "WeakArmorMan";
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_tMonsterDesc.m_fAtk = 30.f;
		m_tMonsterDesc.m_fMaxHP_white = 350.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 400.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Armorman";
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_tMonsterDesc.m_fAtk = 25.f;
		m_tMonsterDesc.m_fMaxHP_white = 250.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 300.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "WeakArmorMan";
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_tMonsterDesc.m_fAtk = 20.f;
		m_tMonsterDesc.m_fMaxHP_white = 150.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 200.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Skullman";
		break;
	case Client::MONSTERTYPE::JOKER:
		m_tMonsterDesc.m_fAtk = 30.f;
		m_tMonsterDesc.m_fMaxHP_white = 500.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 500.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Elite_Joker";
		break;
	case Client::MONSTERTYPE::BIGHANDMAN:
		m_tMonsterDesc.m_fAtk = 25.f;
		m_tMonsterDesc.m_fMaxHP_white = 400.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 400.f;
		m_tMonsterDesc.m_iLifeCount = 1;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Elite_BigHandman";
		break;
	case Client::MONSTERTYPE::BAT:
		m_tMonsterDesc.m_fAtk = 30.f;
		m_tMonsterDesc.m_fMaxHP_white = 1000.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 1500.f;
		m_tMonsterDesc.m_iLifeCount = 2;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Boss_Bat";
		break;
	case Client::MONSTERTYPE::URD:
		m_tMonsterDesc.m_fAtk = 15.f;
		m_tMonsterDesc.m_fMaxHP_white = 1500.f;
		m_tMonsterDesc.m_fMaxParryingGauge = 3000.f;
		m_tMonsterDesc.m_iLifeCount = 2;
		m_tMonsterDesc.m_iMaxParryCount = 10000;
		m_tMonsterDesc.m_szModelKey = "Boss_Urd";
		break;

	}
	m_tMonsterDesc.m_fCurrentHP_white = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fCurrentHP_Green = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fMaxHP_Green = m_tMonsterDesc.m_fMaxHP_white;
	m_tMonsterDesc.m_fHitedTime = 999.f;
	m_tMonsterDesc.m_fParryGaugeRecoveryTime = 0.f;//얘는 차오른 패리 게이지 양에 따라 줄어드는 시간이 달라짐.
	m_tMonsterDesc.m_fRecoveryAlramTime = 5.f;
	m_tMonsterDesc.m_fRecoveryTime = 7.f;
	m_tMonsterDesc.m_fHpBarDisableTime = 15.f;
	m_tMonsterDesc.m_iCueentParryCount = 0.f;
	m_tMonsterDesc.m_fCurrentParryingGauge = 0.f;
	m_tMonsterDesc.m_fRecoveryAmountPercentageFromSecond = 0.3f;
	m_tMonsterDesc.m_fRecoveryMag = 1.f;
	m_tMonsterDesc.m_iDropMemory = 1000;
}

void CStatus_Monster::Free()
{
}



