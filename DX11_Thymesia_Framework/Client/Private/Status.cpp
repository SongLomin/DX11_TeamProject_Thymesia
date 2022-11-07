#include "stdafx.h"
#include "Status.h"

GAMECLASS_C(CStatus)
CLONE_C(CStatus, CComponent)

HRESULT CStatus::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStatus::Initialize(void* pArg)
{
	return S_OK;
}

void CStatus::Start()
{
}

void CStatus::Init_Status(const STATUS_DESC& In_tStatusDesc)
{
	m_tStatusDesc = In_tStatusDesc;
	m_tStatusDesc.fRedRatio = 1.f;
}

void CStatus::Add_Damage(const _float& In_fDamage)
{
	m_tStatusDesc.fCurrentHP -= In_fDamage;

	if (m_tStatusDesc.fCurrentHP < 0.f)
	{
		m_tStatusDesc.fCurrentHP = 0.f;
	}
}

void CStatus::Heal_HP(const _float& In_fHP)
{
	m_tStatusDesc.fCurrentHP += In_fHP;

	if (m_tStatusDesc.fCurrentHP > m_tStatusDesc.fMaxHP)
	{
		m_tStatusDesc.fCurrentHP = m_tStatusDesc.fMaxHP;
	}
}

void CStatus::Add_DashRatio(const _float& In_fDashRatio)
{
	m_tStatusDesc.fDashRatio += In_fDashRatio;
	m_tStatusDesc.fDashRatio = min(1.f, m_tStatusDesc.fDashRatio);
}

void CStatus::On_Swap()
{
	m_tStatusDesc.fSwapTime = m_tStatusDesc.fMaxSwapTime;
}

void CStatus::Add_SwapTime(const _float& In_fTimeDelta)
{
	m_tStatusDesc.fSwapTime += In_fTimeDelta;
}

void CStatus::Free()
{
}
