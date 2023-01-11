#include "stdafx.h"
#include "UI_RadialBlurMask.h"
#include "EasingComponent_Float.h"
#include "GameInstance.h"
#include "GameManager.h"
#include "UI_Utils.h"

GAMECLASS_C(CUI_RadialBlurMask)
CLONE_C(CUI_RadialBlurMask, CGameObject)


HRESULT CUI_RadialBlurMask::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pEasingFloat = Add_Component<CEasingComponent_Float>();

    return S_OK;
}

HRESULT CUI_RadialBlurMask::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_RadialBlurMask::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

    if (m_pEasingFloat.lock()->Is_Lerping())
    {
        _float fRadialBlurAmount = m_pEasingFloat.lock()->Get_Lerp();

        _float3 fPlayerWorldPos;

        XMStoreFloat3(&fPlayerWorldPos,
            GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_WorldPosition());

        GAMEINSTANCE->Set_RadialBlur(fRadialBlurAmount, fPlayerWorldPos);
    }
}

void CUI_RadialBlurMask::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_RadialBlurMask::Set_Radial(_float fStart, _float fTarget, _float fTime, EASING_TYPE eType)
{
    m_pEasingFloat.lock()->Set_Lerp(fStart, fTarget, fTime, eType, CEasingComponent::ONCE);
    m_pEasingFloat.lock()->Callback_LerpEnd += bind(&CUI_RadialBlurMask::Call_OnEndLerp, this);
}

void CUI_RadialBlurMask::Call_OnEndLerp()
{
    Callback_OnEndLerp();
    Callback_OnEndLerp.Clear();
}

void CUI_RadialBlurMask::Free()
{
}

