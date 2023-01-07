#include "stdafx.h"
#include "UrdWeapon.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Effect_Trail_Distortion.h"
#include "Character.h"

GAMECLASS_C(CUrdWeapon);
CLONE_C(CUrdWeapon, CGameObject);

HRESULT CUrdWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUrdWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	TRAIL_DESC TrailDesc;
	ZeroMemory(&TrailDesc, sizeof(TRAIL_DESC));


	m_pModelCom.lock()->Init_Model("Boss_UrdWeapon", "", (_uint)TIMESCALE_LAYER::MONSTER);
	

	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	//TrailDesc.iMaxCnt = 100;
	//TrailDesc.vPos_0 = _float3(0.f, 0.f, 0.f);
	//TrailDesc.vPos_1 = _float3(0.f, 3.f, 0.f);
	//m_pTrailDistortion = GAMEINSTANCE->Add_GameObject<CEffect_Trail_Distortion>(LEVEL_GAMEPLAY, &TrailDesc);
	//m_pTrailDiffuse = GAMEINSTANCE->Add_GameObject<CEffect_Trail>(LEVEL_GAMEPLAY, &TrailDesc);
	
#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
#endif // _USE_THREAD_


	return S_OK;
}

HRESULT CUrdWeapon::Start()
{
	__super::Start();
	//m_pTrailDistortion.lock()->Set_OwnerDesc(m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());
	//m_pTrailDiffuse.lock()->Set_OwnerDesc(m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());
	//
	//m_pTrailDistortion.lock()->Set_TextureIndex(0, 869, 8);//781
	//m_pTrailDiffuse.lock()->Set_TextureIndex(83, 701, 0);
	//
	//m_pTrailDistortion.lock()->Set_Enable(false);
	//m_pTrailDiffuse.lock()->Set_Enable(false);

	return S_OK;
}

void CUrdWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//m_pTrailDistortion.lock()->Update(fTimeDelta, m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());
	//m_pTrailDiffuse.lock()->Update(fTimeDelta, m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());

}

void CUrdWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CUrdWeapon::Thread_PreBeforeRender(_float fTimeDelta)
{
	__super::Thread_PreBeforeRender(fTimeDelta);


	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();
	m_pModelCom.lock()->Update_NvCloth(pDeferredContext, m_pTransformCom.lock()->Get_WorldMatrix());
	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);
}


HRESULT CUrdWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}

_bool CUrdWeapon::Set_TrailEnable(const _bool In_bEnable)
{
	
	return true;

}

void CUrdWeapon::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CUrdWeapon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CUrdWeapon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CUrdWeapon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CUrdWeapon::Free()
{
}