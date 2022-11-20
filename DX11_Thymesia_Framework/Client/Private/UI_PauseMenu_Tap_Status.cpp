#include "stdafx.h"
#include "UI_PauseMenu_Tap_Status.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "CustomUI.h"



GAMECLASS_C(CUI_PauseMenu_Tap_Status)
CLONE_C(CUI_PauseMenu_Tap_Status, CGameObject)

HRESULT CUI_PauseMenu_Tap_Status::Initialize_Prototype()
{
    __super::Initialize_Prototype();


    return S_OK;
}

HRESULT CUI_PauseMenu_Tap_Status::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_tUIDesc.fSizeX = 389.f;
    m_tUIDesc.fSizeY = 658.f;
    m_tUIDesc.fX = 239.f;
    m_tUIDesc.fY = 116.f + (m_tUIDesc.fSizeY * 0.5f);
    m_tUIDesc.fDepth = 0.7f;

    m_pStatusBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pStatusBG.lock()->Set_Texture("Tap_Status_BG");

    m_vecChildUI.push_back(m_pStatusBG);


    Create_Font();

    return S_OK;
}

HRESULT CUI_PauseMenu_Tap_Status::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_PauseMenu_Tap_Status::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CUI_PauseMenu_Tap_Status::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    for (_uint i = 0; i < TAP_STATUS_CNT; i++)
    {
        GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_TextInfo[i]);

        GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_StatusTextInfo[i]);

    }
    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_CorvusTextInfo);

}

HRESULT CUI_PauseMenu_Tap_Status::Render()
{
    //
    return S_OK;
}

void CUI_PauseMenu_Tap_Status::Create_Font()
{
    
    m_szStatusText.push_back(L"Level");
    m_szStatusText.push_back(L"Attack Damage");
    m_szStatusText.push_back(L"Wound Count");
    m_szStatusText.push_back(L"Claw Damage");
    m_szStatusText.push_back(L"Health Point");
    m_szStatusText.push_back(L"Energy");
    m_szStatusText.push_back(L"Strength");
    m_szStatusText.push_back(L"Vitality");
    m_szStatusText.push_back(L"Plague");
    m_szStatusText.push_back(L"Feather");

    UI_DESC tUIDesc;    
    for (_uint i = 0; i < TAP_STATUS_CNT; i++)
    {
        tUIDesc.fX = 126.f;
        tUIDesc.fY = 263.f + ((_float)i * 43.f);
        tUIDesc.fSizeX = 16.f;
        tUIDesc.fSizeY = 16.f;
        tUIDesc.fDepth = 0.1f;

        m_pTextIcon[i] = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tUIDesc);
        m_pTextIcon[i].lock()->Set_Texture("Font_Diamond");
        m_vecChildUI.push_back(m_pTextIcon[i]);
    }
    
    //Font Decoration
    for (_uint i = 0; i < TAP_STATUS_CNT; i++)
    {
        tUIDesc.fX = 330.f;
        tUIDesc.fY = 284.f + ((_float)i * 43.f);
        tUIDesc.fSizeX = 345.f;
        tUIDesc.fSizeY = 3.f;
        tUIDesc.fDepth = 0.1f;

        m_pFontDecoration[i] = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tUIDesc);
        m_pFontDecoration[i].lock()->Set_Texture("Font_Decoration2");
        m_vecChildUI.push_back(m_pFontDecoration[i]);

    }

    //Font Infomaiton
    for (_uint i = 0; i < TAP_STATUS_CNT; i++)
    {
        m_TextInfo[i].bAlways = false;
        m_TextInfo[i].bCenterAlign = false;
        m_TextInfo[i].fRotation = 0.f;
        m_TextInfo[i].szText = m_szStatusText[i];
        m_TextInfo[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
        m_TextInfo[i].vPosition = _float2(157.f, 253.f + ((_float)i * 43.f));
        m_TextInfo[i].vScale = _float2(0.5f, 0.5f);


        m_StatusTextInfo[i].bAlways = false;
        m_StatusTextInfo[i].bCenterAlign = false;
        m_StatusTextInfo[i].fRotation = 0.f;
        m_StatusTextInfo[i].szText = L"0";
        m_StatusTextInfo[i].vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
        m_StatusTextInfo[i].vPosition = _float2(458.f, 253.f + ((_float)i * 43.f));
        m_StatusTextInfo[i].vScale = _float2(0.5f, 0.5f);


    }

    m_CorvusTextInfo.bAlways = false;
    m_CorvusTextInfo.bCenterAlign = true;
    m_CorvusTextInfo.fRotation = 0.f;
    m_CorvusTextInfo.szText = L"Corvus";
    m_CorvusTextInfo.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_CorvusTextInfo.vPosition = _float2(311.f, 174.f);
    m_CorvusTextInfo.vScale = _float2(1.3f, 1.3f);
    

/*
    UI_DESC tUIDesc;
    tUIDesc.fDepth = 0.3f;
    tUIDesc.fSizeX = 

    m_pUICorvusTextDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    


    m_pFontDecoration->lock()->Set_Texture("PageIndex_Indicator_Decoration");

    m_vecChildUI.push_back(m_pUICorvusTextDecoration);
  */  
}

