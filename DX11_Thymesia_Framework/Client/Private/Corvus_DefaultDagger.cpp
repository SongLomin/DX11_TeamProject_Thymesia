#include "stdafx.h"
#include "Corvus_DefaultDagger.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Effect_Trail_Distortion.h"
#include "Effect_Trail_EyeLight.h"
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

#ifdef _DAGGER_TRAIL_
	TRAIL_DESC TrailDesc;
	ZeroMemory(&TrailDesc, sizeof(TRAIL_DESC));

	TrailDesc.iMaxCnt = 240;
	TrailDesc.vPos_0 = _float3(0.f, 0.f, 0.f);
	TrailDesc.vPos_1 = _float3(0.5f, 0.f, 0.f);
	m_pTrailEffect = GAMEINSTANCE->Add_GameObject<CEffect_Trail_EyeLight>(LEVEL_GAMEPLAY, &TrailDesc);
#endif // _DAGGER_TRAIL_
	return S_OK;
}

HRESULT CCorvus_DefaultDagger::Start()
{
	__super::Start();

#ifdef _DAGGER_TRAIL_
	//m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());
#endif // _DAGGER_TRAIL_

	return S_OK;
}

void CCorvus_DefaultDagger::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#ifdef _DAGGER_TRAIL_
	m_pTrailEffect.lock()->Update(fTimeDelta, m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());
#endif // _DAGGER_TRAIL_
}

void CCorvus_DefaultDagger::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCorvus_DefaultDagger::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}
_bool CCorvus_DefaultDagger::Set_TrailEnable(const _bool In_bEnable)
{
	if (!m_pTrailEffect.lock())
		return false;

	return m_pTrailEffect.lock()->Set_Enable(In_bEnable);

}

void CCorvus_DefaultDagger::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CCorvus_DefaultDagger::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CCorvus_DefaultDagger::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CCorvus_DefaultDagger::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CCorvus_DefaultDagger::Free()
{
}