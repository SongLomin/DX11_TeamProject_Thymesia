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

void CStatus::Free()
{
}
