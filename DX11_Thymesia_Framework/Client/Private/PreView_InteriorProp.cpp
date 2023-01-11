#include "stdafx.h"
#include "PreView_InteriorProp.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Collider.h"

#include "GameManager.h"

GAMECLASS_C(CPreView_InteriorProp);
CLONE_C(CPreView_InteriorProp, CGameObject);

HRESULT CPreView_InteriorProp::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPreView_InteriorProp::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom      = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    return S_OK;
}

HRESULT CPreView_InteriorProp::Start()
{
    __super::Start();

    return S_OK;
}

void CPreView_InteriorProp::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CPreView_InteriorProp::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CPreView_InteriorProp::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(SetUp_ShaderResource()))
        return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

		m_pShaderCom.lock()->Begin(15, pDeviceContext);
		m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

    __super::Render(pDeviceContext);

	return S_OK;
}

void CPreView_InteriorProp::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_bCheckBuild = false;

    m_pCollisionObject = pOtherCollider.lock()->Get_Owner();
}

void CPreView_InteriorProp::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_bCheckBuild = false;
}

void CPreView_InteriorProp::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_bCheckBuild = true;

    m_pCollisionObject = weak_ptr<CGameObject>();
}

void CPreView_InteriorProp::Set_Offset(_vector _vOffset)
{
    XMStoreFloat3(&m_vOffset, _vOffset);

    weak_ptr<CGameObject> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
    if (pPlayer.lock())
    {
        weak_ptr<CTransform> pPlayerTransform = pPlayer.lock()->Get_Transform();

        _vector vPos = pPlayerTransform.lock()->Get_Position();
        vPos += XMLoadFloat3(&m_vOffset);

        m_pTransformCom.lock()->Set_Position(vPos);
    }
}

void CPreView_InteriorProp::Set_Model(string _szModelKey)
{
    m_pModelCom.lock()->Init_Model(_szModelKey.c_str());

    if (!m_pModelCom.lock()->Get_ModelData().lock())
        return;

    MESH_VTX_INFO tInfo = m_pModelCom.lock()->Get_MeshVertexInfo();

    Remove_Components<CCollider>();
    m_pColliderCom = Add_Component<CCollider>();

    COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::INTERIOR;
    ColliderDesc.vScale       = _float3((tInfo.vMax.x - tInfo.vMin.x), (tInfo.vMax.y - tInfo.vMin.y), (tInfo.vMax.z - tInfo.vMin.z));
    ColliderDesc.vRotation    = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, tInfo.vCenter.y, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::OBB, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

HRESULT CPreView_InteriorProp::SetUp_ShaderResource()
{
    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

void CPreView_InteriorProp::Free()
{
}
