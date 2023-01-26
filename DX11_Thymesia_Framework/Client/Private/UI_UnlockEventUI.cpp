#include "stdafx.h"
#include "UI_UnlockEventUI.h"

HRESULT CUI_UnlockEventUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_UnlockEventUI::Initialize(void* pArg)
{
    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

    return S_OK;
}

HRESULT CUI_UnlockEventUI::Start()
{
    return E_NOTIMPL;
}

void CUI_UnlockEventUI::Tick(_float fTimeDelta)
{
}

void CUI_UnlockEventUI::LateTick(_float fTimeDelta)
{
}
