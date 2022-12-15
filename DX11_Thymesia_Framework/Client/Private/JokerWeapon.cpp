#include "stdafx.h"
#include "JokerWeapon.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Effect_Trail_Distortion.h"
#include "Character.h"

GAMECLASS_C(CJokerWeapon);
CLONE_C(CJokerWeapon, CGameObject);

HRESULT CJokerWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CJokerWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	//m_pTrailDiffuse = GAMEINSTANCE->Add_GameObject<CEffect_Trail>(LEVEL_GAMEPLAY, &TrailDesc);
	
	return S_OK;
}

HRESULT CJokerWeapon::Start()
{
	__super::Start();


	//m_pTrailDiffuse.lock()->Set_TextureIndex(83, 701, 0);

	return S_OK;
}

void CJokerWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//m_pTrailDiffuse.lock()->Update(fTimeDelta, m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());

}

void CJokerWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CJokerWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);
	return S_OK;
}


void CJokerWeapon::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CJokerWeapon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CJokerWeapon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CJokerWeapon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CJokerWeapon::Free()
{
}