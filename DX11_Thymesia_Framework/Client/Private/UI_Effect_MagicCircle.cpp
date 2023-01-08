#include "stdafx.h"
#include "UI_Effect_MagicCircle.h"
#include "UI_Utils.h"
#include "UIManager.h"

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

    Set_Texture("UVMask");

    m_iPassIndex = 1;
    m_iDeffuseIndex = 107;

    Set_Depth(0.5f);

    SetUp_Animaiton();

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

void CUI_Effect_MagicCircle::Play(_bool bRepeat)
{
    POINT	tMousePt;

    GetCursorPos(&tMousePt);
    ClientToScreen(g_hWnd, &tMousePt);

    Set_UIPosition((_float)tMousePt.x, (_float)tMousePt.y);

    __super::Play(bRepeat);
}

void CUI_Effect_MagicCircle::SetUp_Animaiton()
{   
    UICLIPDESC tUIClipDesc;

    tUIClipDesc._fClipTime = 1.f;
    tUIClipDesc._eAlphaEasingType = EASING_TYPE::QUAD_IN;
    tUIClipDesc._fStartSize = _float2(128.f, 128.f);
    tUIClipDesc._fTargetSize = _float2(128.f, 128.f);
    tUIClipDesc._fStartAlpha = 0.f;
    tUIClipDesc._fTargetAlpha = 1.f;
    tUIClipDesc._eUseCondition |= (_flag)UI_USE_CONDITION::UI_USE_ALPHA;
    tUIClipDesc._eUseCondition |= (_flag)UI_USE_CONDITION::UI_USE_SIZE;

    Add_Clip(tUIClipDesc);
}
