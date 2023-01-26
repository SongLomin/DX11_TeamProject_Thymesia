#include "stdafx.h"
#include "UI_Container.h"

HRESULT CUI_Container::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Container::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    return S_OK;
}

HRESULT CUI_Container::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Container::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Container::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_Container::Render(ID3D11DeviceContext* pDeviceContext)
{

    return S_OK;
}


