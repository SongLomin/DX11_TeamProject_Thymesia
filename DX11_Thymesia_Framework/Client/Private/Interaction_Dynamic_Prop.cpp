#include "stdafx.h"
#include "Interaction_Dynamic_Prop.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

GAMECLASS_C(CInteraction_Dynamic_Prop);
CLONE_C(CInteraction_Dynamic_Prop, CGameObject);

HRESULT CInteraction_Dynamic_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Dynamic_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    return S_OK;
}

HRESULT CInteraction_Dynamic_Prop::Start()
{
    return S_OK;
}

void CInteraction_Dynamic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Dynamic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Dynamic_Prop::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CInteraction_Dynamic_Prop::Free()
{
}
