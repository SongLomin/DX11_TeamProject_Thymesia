#include "stdafx.h"
#include "UI_Effect_MagicCircle.h"
#include "UI_Utils.h"


GAMECLASS_C(CUI_Effect_MagicCircle)
CLONE_C(CUI_Effect_MagicCircle, CGameObject)


HRESULT CUI_Effect_MagicCircle::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_Effect_MagicCircle::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    m_iPassIndex = 1;

    return S_OK;
}

HRESULT CUI_Effect_MagicCircle::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Effect_MagicCircle::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_Effect_MagicCircle::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}
