#include "stdafx.h"
#include "PreView_Prop.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

GAMECLASS_C(CPreView_Prop);
CLONE_C(CPreView_Prop, CGameObject);

HRESULT CPreView_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPreView_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    return S_OK;
}

HRESULT CPreView_Prop::Start()
{
    __super::Start();

    return S_OK;
}

void CPreView_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CPreView_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CPreView_Prop::Render()
{
    return __super::Render();
}

void CPreView_Prop::Free()
{
}
