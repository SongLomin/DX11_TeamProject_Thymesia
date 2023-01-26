#include "stdafx.h"
#include "MobWeapon.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"

GAMECLASS_C(CMobWeapon);
CLONE_C(CMobWeapon, CGameObject);

HRESULT CMobWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMobWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}

HRESULT CMobWeapon::Start()
{
	__super::Start();
	CWeapon::Disable_Weapon();
	return S_OK;
}

void CMobWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CMobWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CMobWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);
	return S_OK;
}

void CMobWeapon::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CMobWeapon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CMobWeapon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CMobWeapon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CMobWeapon::Free()
{
}