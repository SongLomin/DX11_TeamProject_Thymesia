#include "stdafx.h"
#include "Interaction_Prop.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

GAMECLASS_C(CInteraction_Prop);

HRESULT CInteraction_Prop::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CInteraction_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    return S_OK;
}

HRESULT CInteraction_Prop::Start()
{
    return S_OK;
}

void CInteraction_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Prop::Render()
{
    __super::Render();

    return S_OK;
}

void CInteraction_Prop::Free()
{
}
