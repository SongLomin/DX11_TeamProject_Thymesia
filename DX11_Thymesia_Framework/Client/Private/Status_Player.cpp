#include "stdafx.h"
#include "Status_Player.h"
#include "Status.h"


GAMECLASS_C(CStatus_Player)
CLONE_C(CStatus_Player, CComponent)


HRESULT CStatus_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStatus_Player::Initialize(void* pArg)
{
	Init_Status(pArg);

	return S_OK;
}

void CStatus_Player::Start()
{
	__super::Start();
}

void CStatus_Player::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fPotionTime -= fTimeDelta;

	if (m_fPotionTime >= 0.f)
	{
		m_tDesc.m_fCurrentHP += ((m_tDesc.m_fMaxHP * m_PotionDesc[m_iCurrentPotionIndex].m_fHealingAmount) /
			m_PotionDesc[m_iCurrentPotionIndex].m_fHealingTime) * fTimeDelta;
	}
}

void CStatus_Player::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

_bool CStatus_Player::Is_Dead()
{
	return m_tDesc.m_fCurrentHP <= 0.f ? true : false;
}

void CStatus_Player::Init_Status(const void* pArg)
{
	m_tDesc.m_fMaxHP = 300.f;
	m_tDesc.m_fMaxMP = 300.f;
	m_tDesc.m_fNormalAtk = 25.f;
	m_tDesc.m_fPlagueAtk = 25.f;
	m_tDesc.m_fFeatherAtk = 30.f;
	m_tDesc.m_fParryingAtk = 50.f;


	m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_iMaxPotion = 3;
	m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_fHealingAmount = 0.3f;
	m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_fHealingTime = 0.f;//0초면 바로 회복됨.
	
	m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_iMaxPotion = 0;
	m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_fHealingAmount = 0.5f;
	m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_fHealingTime = 5.f;//0초면 바로 회복됨.

	m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_iMaxPotion = 0;
	m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_fHealingAmount = 0.2f;
	m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_fHealingTime = 0.1f;//0초면 바로 회복됨.

	m_iCurrentPotionIndex = 0;
	Full_Recovery();
}

void CStatus_Player::Add_Damage(const _float& In_fDamage)
{
	Decrease_HP(m_tDesc.m_fCurrentHP, In_fDamage);
	
	Callback_ChangeHP(m_tDesc.m_fCurrentHP);
}

void CStatus_Player::Full_Recovery()
{
	m_tDesc.m_fCurrentHP = m_tDesc.m_fMaxHP;
	m_tDesc.m_fCurrentMP = m_tDesc.m_fMaxMP;
	m_tDesc.m_iCurrentFeather = m_tDesc.m_iMaxFeather;


	m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_iCurrentPotion  =
	m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_iMaxPotion;
	
	m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_iCurrentPotion =
	m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_iMaxPotion;


	m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_iCurrentPotion =
	m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_iMaxPotion;
}

void CStatus_Player::Get_Desc(void* Out_pDesc)
{
	memcpy(Out_pDesc, &m_tDesc, sizeof(PLAYERDESC));
}

void CStatus_Player::Heal_Player(const _float fAmount)
{
	m_tDesc.m_fCurrentHP += fAmount;
	if (m_tDesc.m_fCurrentHP >= m_tDesc.m_fMaxHP)
		m_tDesc.m_fCurrentHP = m_tDesc.m_fMaxHP;

	Callback_ChangeHP(m_tDesc.m_fCurrentHP);
}

