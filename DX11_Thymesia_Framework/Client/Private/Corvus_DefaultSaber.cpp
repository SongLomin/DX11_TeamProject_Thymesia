#include "stdafx.h"
#include "Corvus_DefaultSaber.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"
#include "Effect_Trail.h"
#include "VIBuffer_Trail.h"

GAMECLASS_C(CCorvus_DefaultSaber);
CLONE_C(CCorvus_DefaultSaber, CGameObject);

HRESULT CCorvus_DefaultSaber::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCorvus_DefaultSaber::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pModelCom.lock()->Init_Model("CorvusDefaultSaber", "", (_uint)TIMESCALE_LAYER::PLAYER);

	

	return S_OK;
}

HRESULT CCorvus_DefaultSaber::Start()
{
	__super::Start();

	return S_OK;
}

void CCorvus_DefaultSaber::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//m_pTrailEffect.lock()->Update(fTimeDelta, m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());


}

void CCorvus_DefaultSaber::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCorvus_DefaultSaber::Render()
{
	__super::Render();

	return S_OK;
}


void CCorvus_DefaultSaber::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CCorvus_DefaultSaber::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CCorvus_DefaultSaber::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CCorvus_DefaultSaber::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CCorvus_DefaultSaber::Free()
{
}