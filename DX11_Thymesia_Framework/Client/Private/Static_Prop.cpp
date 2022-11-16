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

void CStatic_Prop::Free()
{
}
