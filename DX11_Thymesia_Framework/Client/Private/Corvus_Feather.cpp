#include "stdafx.h"
#include "Corvus_Feather.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"
#include "Effect_Trail.h"
#include "VIBuffer_Trail.h"

GAMECLASS_C(CCorvus_Feather);
CLONE_C(CCorvus_Feather, CGameObject);

HRESULT CCorvus_Feather::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCorvus_Feather::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pModelCom.lock()->Init_Model("CorvusFeather", "", (_uint)TIMESCALE_LAYER::PLAYER);

	m_pPhysXColliderCom = Add_Component<CPhysXCollider>();
	PHYSXCOLLIDERDESC PhysXDesc;
	Preset::PhysXColliderDesc::PlayerWeaponSetting(PhysXDesc, m_pTransformCom.lock());

	m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
	m_pPhysXColliderCom.lock()->CreatePhysXActor(PhysXDesc);
	m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();

	return S_OK;
}

HRESULT CCorvus_Feather::Start()
{
	__super::Start();

	return S_OK;
}

void CCorvus_Feather::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//m_pTrailEffect.lock()->Update(fTimeDelta, m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());


}

void CCorvus_Feather::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

HRESULT CCorvus_Feather::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}


void CCorvus_Feather::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CCorvus_Feather::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CCorvus_Feather::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CCorvus_Feather::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CCorvus_Feather::Free()
{
}