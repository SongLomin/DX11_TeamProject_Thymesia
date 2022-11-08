#include "stdafx.h"
#include "Player_FeatherUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "Player_ProgressBar.h"


GAMECLASS_C(CPlayer_FeatherUI)
CLONE_C(CPlayer_FeatherUI, CGameObject);

HRESULT CPlayer_FeatherUI::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CPlayer_FeatherUI::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (pArg != nullptr)
        memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));
    else
    {
        Set_UIPosition(1260.f, 785.f, 40.f, 40.f);
        m_tUIDesc.fDepth = 0.f;
    }
    m_iMaxFeather = 0;
    m_iCurrentFeather = 0;

    m_pFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pFrame.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Frame");
    m_pFrame.lock()->Set_Depth(0.2f);

    m_pFrameBorder = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pFrameBorder.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_FrameBorder");
    m_pFrameBorder.lock()->Set_Depth(0.0f);
    m_pFrameBorder.lock()->Set_PassIndex(10);
    

    m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pIcon.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Feather");
    m_pIcon.lock()->Set_Depth(0.0f);


    m_pFontBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pFontBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Font_BG");
    m_pFontBG.lock()->Set_UIPosition(m_tUIDesc.fX + 40.f, m_tUIDesc.fY, 80.f, 28.f);
    m_pFontBG.lock()->Set_Depth(0.1f);


    //left 
    m_tCurrentFeatherTextInfo.bAlways = false;
    m_tCurrentFeatherTextInfo.bCenterAlign = false;
    m_tCurrentFeatherTextInfo.fRotation = 0.f;
    m_tCurrentFeatherTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
    m_tCurrentFeatherTextInfo.vScale = _float2(0.5, 0.5f);
    m_tCurrentFeatherTextInfo.vPosition = _float2(m_tUIDesc.fX + 20.f, m_tUIDesc.fY - 10.f);


    m_tMaxFeatherTextInfo.bAlways = false;
    m_tMaxFeatherTextInfo.bCenterAlign = false;
    m_tMaxFeatherTextInfo.fRotation = 0.f;
    m_tMaxFeatherTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
    m_tMaxFeatherTextInfo.vScale = _float2(0.5, 0.5f);
    m_tMaxFeatherTextInfo.vPosition = _float2(m_tUIDesc.fX + 40.f, m_tUIDesc.fY - 10.f);


    m_fRatio = 1.f;

    return S_OK;
}

HRESULT CPlayer_FeatherUI::Start()
{
    __super::Start();

    return S_OK;
}

void CPlayer_FeatherUI::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

#ifdef _DEBUG
    if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
    {
        Set_CurrentFeather(0);

    }
    if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
    {
        Set_CurrentFeather(1);
    }
    if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
    {
        m_fRatio -= 0.3f * fTimeDelta;
    }
    if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
    {
        m_fRatio += 0.3f * fTimeDelta;
    }

#endif // _DEBUG
    m_pFrameBorder.lock()->Set_Ratio(m_fRatio);

    m_tCurrentFeatherTextInfo.szText = to_wstring(m_iCurrentFeather);
    m_tMaxFeatherTextInfo.szText = L"/ ";
    m_tMaxFeatherTextInfo.szText.append(to_wstring(m_iMaxFeather));
}

void CPlayer_FeatherUI::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tCurrentFeatherTextInfo);
    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tMaxFeatherTextInfo);

}

HRESULT CPlayer_FeatherUI::Render()
{
    return S_OK;
}

_uint CPlayer_FeatherUI::Get_CrrrentFeather()
{
    return m_iCurrentFeather;
}

void CPlayer_FeatherUI::Set_MaxFeather(_uint _iMaxFeather)
{
    m_iMaxFeather = _iMaxFeather;
}

void CPlayer_FeatherUI::Set_CurrentFeather(_uint _iCurrentFeather)
{
    m_iCurrentFeather = _iCurrentFeather;

    if (m_iCurrentFeather == 0)
        m_tCurrentFeatherTextInfo.vColor = _float4(1.f, 0.f, 0.f, 1.f);
    else
        m_tCurrentFeatherTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
}


