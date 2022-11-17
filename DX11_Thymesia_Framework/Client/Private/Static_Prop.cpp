#include "stdafx.h"
#include "Static_Prop.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

#include "PhysXCollider.h"

GAMECLASS_C(CStatic_Prop);
CLONE_C(CStatic_Prop, CGameObject);

HRESULT CStatic_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStatic_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

    return S_OK;
}

HRESULT CStatic_Prop::Start()
{
    __super::Start();

    return S_OK;
}

void CStatic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CStatic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CStatic_Prop::Render()
{
    return __super::Render();
}

HRESULT CStatic_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_pShaderCom.lock()->Begin(1);
        m_pModelCom.lock()->Render_Mesh(i);
    }

    return S_OK;
}

void CStatic_Prop::Free()
{
}
