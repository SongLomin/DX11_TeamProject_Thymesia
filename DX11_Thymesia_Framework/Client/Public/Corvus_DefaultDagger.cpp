#include "stdafx.h"
#include "Corvus_DefaultDagger.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"

GAMECLASS_C(CCorvus_DefaultDagger);
CLONE_C(CCorvus_DefaultDagger, CGameObject);

HRESULT CCorvus_DefaultDagger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCorvus_DefaultDagger::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pModelCom.lock()->Init_Model("CorvusDefaultDagger", "", (_uint)TIMESCALE_LAYER::PLAYER);


	return S_OK;
}

HRESULT CCorvus_DefaultDagger::Start()
{
	__super::Start();

	return S_OK;
}

void CCorvus_DefaultDagger::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CCorvus_DefaultDagger::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCorvus_DefaultDagger::Render()
{
	__super::Render();

	return S_OK;
}
void CCorvus_DefaultDagger::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CCorvus_DefaultDagger::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pOtherCollider);
}

void CCorvus_DefaultDagger::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pOtherCollider);
}

void CCorvus_DefaultDagger::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pOtherCollider);
}

void CCorvus_DefaultDagger::Free()
{
}