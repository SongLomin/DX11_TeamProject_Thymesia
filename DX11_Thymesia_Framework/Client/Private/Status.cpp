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

void CStatus::Decrease_HP(_float& fCurrentHP, const _float& In_fDamage)
{
	fCurrentHP -= In_fDamage;
	if (fCurrentHP <= 0.f)
		fCurrentHP = 0.f;

}

_bool CStatus::Is_Dead()
{
	return _bool();
}

void CStatus::Init_Status(const void* pArg)
{
}

void CStatus::Add_Damage(const _float& In_fDamage, ATTACK_OPTION eAttackOption)
{

}

void CStatus::Get_Desc(void* Out_pDesc)
{
}

void CStatus::Free()
{
}
