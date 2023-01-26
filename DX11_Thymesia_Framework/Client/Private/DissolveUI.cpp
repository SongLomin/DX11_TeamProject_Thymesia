#include "stdafx.h"
#include "DissolveUI.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "EasingComponent_Float.h"

GAMECLASS_C(CDissolveUI);
CLONE_C(CDissolveUI, CGameObject);


HRESULT CDissolveUI::Initialize_Prototype()
{
    __super::Initialize_Prototype();


    return S_OK;
}

HRESULT CDissolveUI::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pDissolveTexture = Add_Component<CTexture>();
    m_pDissolveTexture.lock()->Use_Texture("Dissolve_1");

    m_fRatio = 0.f;
    m_iPassIndex = 4;

    SetUp_Component();

    return S_OK;
}

HRESULT CDissolveUI::Start()
{
    __super::Start();

    return S_OK;
}

void CDissolveUI::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    //TODO : Dissolve 아이콘 테스트 코드입니다.
#ifdef _DEBUG
    if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
    {
        m_fRatio -= 0.5f * fTimeDelta;
        if (m_fRatio < 0.f)
            m_fRatio = 0.f;
    }
    if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
    {
        m_fRatio += 0.5f * fTimeDelta;
        if (m_fRatio >= 1.f)
            m_fRatio = 1.f;
    }

    if (KEY_INPUT(KEY::NUM0, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 0;
    }
    if (KEY_INPUT(KEY::NUM1, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 1;
    }
    if (KEY_INPUT(KEY::NUM2, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 2;
    }
    if (KEY_INPUT(KEY::NUM3, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 3;
    }
    if (KEY_INPUT(KEY::NUM4, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 4;
    }
#endif


}

void CDissolveUI::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CDissolveUI::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CDissolveUI::SetUp_Component()
{
    m_pEasingFloat = Add_Component<CEasingComponent_Float>();
}

HRESULT CDissolveUI::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pDissolveTexture.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", m_iTextureIndex);

    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float));

    return S_OK;
}
void CDissolveUI::Start_FadeIn(_float fStart, _float fTarget, _float fTime)
{
    m_pEasingFloat.lock()->Set_Lerp(fStart, fTarget, fTime, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);

    m_pEasingFloat.lock()->Callback_LerpEnd += bind(&CDissolveUI::Call_EndFadeIn, this);

}

void CDissolveUI::Start_FadeOut(_float fStart, _float fTarget, _float fTime)
{
   m_pEasingFloat.lock()->Set_Lerp(fStart, fTarget, fTime, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE);

   m_pEasingFloat.lock()->Callback_LerpEnd += bind(&CDissolveUI::Call_EndFadeOut, this);
}

void CDissolveUI::Call_EndFadeIn()
{
    Callback_EndFadeIn();

    Callback_EndFadeIn.Clear();
}

void CDissolveUI::Call_EndFadeOut()
{
    Callback_EndFadeOut();

    Callback_EndFadeOut.Clear();

}

void CDissolveUI::Free()
{
    __super::Free();
}
