#include "stdafx.h"
#include "Corvus_DefaultDagger.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Effect_Trail.h"
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

	TRAIL_DESC TrailDesc;
	ZeroMemory(&TrailDesc, sizeof(TRAIL_DESC));

	TrailDesc.iMaxCnt = 120;
	TrailDesc.vPos_0 = _float3(0.f, 1.f, 0.f);
	TrailDesc.vPos_1 = _float3(0.f, -0.3f, 0.f);
	m_pTrailEffect = GAMEINSTANCE->Add_GameObject<CEffect_Trail>(LEVEL_GAMEPLAY, &TrailDesc);

	return S_OK;
}

HRESULT CCorvus_DefaultDagger::Start()
{
	__super::Start();
	m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());


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