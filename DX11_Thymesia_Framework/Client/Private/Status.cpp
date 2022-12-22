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

void CStatus::Decrease_HP(_float& InOut_fCurrentHP, const _float In_fDamage)
{
	if (InOut_fCurrentHP <= In_fDamage)
		InOut_fCurrentHP = 0.f;
	else
		InOut_fCurrentHP -= In_fDamage;
}

void CStatus::Increase_HP(_float& InOut_fCurrentHP, const _float In_fMaxHP, const _float In_fAmount)
{
	InOut_fCurrentHP += In_fAmount;
	if (InOut_fCurrentHP >= In_fMaxHP)
		InOut_fCurrentHP = In_fMaxHP;
}



_bool CStatus::Is_Dead()
{
	return _bool();
}

void CStatus::Init_Status(const void* pArg)
{
}

void CStatus::Add_Damage(const _float In_fDamage, ATTACK_OPTION eAttackOption)
{

}

void CStatus::Get_Desc(void* Out_pDesc)
{
}

void CStatus::Set_Desc(void* In_Desc)
{
}

void CStatus::Full_Recovery()
{
}


void CStatus::Free()
{
}