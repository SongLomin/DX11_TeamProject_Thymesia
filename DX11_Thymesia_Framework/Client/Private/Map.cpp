#include "stdafx.h"
#include "Map.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"

GAMECLASS_C(CMap);
CLONE_C(CMap, CGameObject);

HRESULT CMap::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    
    return S_OK;
}

HRESULT CMap::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    m_pNaviMeshCom = Add_Component<CNavigation>();
    
    return S_OK;
}

HRESULT CMap::Start()
{
    __super::Start();

    return S_OK;
}

void CMap::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CMap::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, Weak_Cast<CGameObject>(m_this));

}

HRESULT CMap::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CMap::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    /*CallBack_Bind_SRV(m_pShaderCom, "");

    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_pShaderCom.lock()->Begin(1);

        m_pModelCom.lock()->Render_Mesh(i);
    }*/

    return S_OK;
}



HRESULT CMap::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    _vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

    m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

}

void CMap::Free()
{
}
