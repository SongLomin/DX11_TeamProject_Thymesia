#include "stdafx.h"
#include "PreView_InteriorProp.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "PhysXCollider.h"

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

    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

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

    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, m_fPhyxOffset, 0.f, 0.f));
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

		m_pShaderCom.lock()->Begin(13, pDeviceContext);
		m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

    __super::Render(pDeviceContext);

	return S_OK;
}

void CPreView_InteriorProp::PhysXCollisionEnter(weak_ptr<CPhysXCollider> pOtherCollider)
{
    m_bCheckBuild = false;
}

void CPreView_InteriorProp::PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider)
{
    m_bCheckBuild = false;
}

void CPreView_InteriorProp::PhysXCollisionExit(weak_ptr<CPhysXCollider> pOtherCollider)
{
    m_bCheckBuild = true;
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

    PHYSXCOLLIDERDESC tDesc;
    Preset::PhysXColliderDesc::StaticInteriorBoxDefaultSetting(tDesc, m_pTransformCom, tInfo, &m_fPhyxOffset);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, m_fPhyxOffset, 0.f, 0.f));
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
