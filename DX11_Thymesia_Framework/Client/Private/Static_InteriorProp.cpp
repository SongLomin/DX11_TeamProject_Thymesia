#include "stdafx.h"
#include "Static_InteriorProp.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "PhysXCollider.h"
#include "Collider.h"

#include "Client_Presets.h"
#include "imgui.h"

GAMECLASS_C(CStatic_InteriorProp);
CLONE_C(CStatic_InteriorProp, CGameObject);

HRESULT CStatic_InteriorProp::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStatic_InteriorProp::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();
    m_pColliderCom      = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

    return S_OK;
}

HRESULT CStatic_InteriorProp::Start()
{
    __super::Start();

    MESH_VTX_INFO tInfo = m_pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

    XMStoreFloat3(&m_vCenterOffset, (XMLoadFloat3(&tInfo.vMin) + XMLoadFloat3(&tInfo.vMax)) / 2.f);
    tInfo.vCenter = m_vCenterOffset;
    m_fCullingOffsetRange = XMVectorGetX(XMVector3Length(XMVectorAbs(XMLoadFloat3(&tInfo.vMax)) + XMVectorAbs(XMLoadFloat3(&tInfo.vMin))));

    PHYSXCOLLIDERDESC tDesc;
    Preset::PhysXColliderDesc::StaticInteriorBoxDefaultSetting(tDesc, m_pTransformCom, tInfo, &m_fPhyxOffset);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, m_fPhyxOffset, 0.f, 0.f));

    COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::INTERIOR;
    ColliderDesc.vScale       = _float3((tInfo.vMax.x - tInfo.vMin.x), (tInfo.vMax.y - tInfo.vMin.y), (tInfo.vMax.z - tInfo.vMin.z));
    ColliderDesc.vRotation    = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, tInfo.vCenter.y, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::OBB, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    return S_OK;
}

void CStatic_InteriorProp::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CStatic_InteriorProp::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CStatic_InteriorProp::Before_Render(_float fTimeDelta)
{
    __super::Before_Render(fTimeDelta);
}

HRESULT CStatic_InteriorProp::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource()))
        return E_FAIL;

    m_iPassIndex = 3;
    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
		_flag BindTextureFlag(0);

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			BindTextureFlag |= (1 << aiTextureType_NORMALS);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		{
			BindTextureFlag |= (1 << aiTextureType_SPECULAR);
		}
		else
		{
			BEGIN_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
			END_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
		}

		m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, m_bInvisibility, false, false);

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return __super::Render(pDeviceContext);
}

HRESULT CStatic_InteriorProp::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_pShaderCom.lock()->Begin(1, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

void CStatic_InteriorProp::PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider)
{

}

HRESULT CStatic_InteriorProp::SetUp_ShaderResource()
{
    if (FAILED(CProp::SetUp_ShaderResource()))
        return E_FAIL; 

    return S_OK;
}

void CStatic_InteriorProp::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);
}

void CStatic_InteriorProp::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);
}

void CStatic_InteriorProp::Free()
{
}
