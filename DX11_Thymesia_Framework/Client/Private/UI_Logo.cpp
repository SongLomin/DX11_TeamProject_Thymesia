#include "stdafx.h"
#include "UI_Logo.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "HUD_Hover.h"
#include "Fader.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "UIManager.h"



GAMECLASS_C(CUI_Logo)
CLONE_C(CUI_Logo, CGameObject)



HRESULT CUI_Logo::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_Logo::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_fontPath[LOGO_BUTTON_CONTINUE_GAME] = "MainMenu_SelectableButton_ContinueGame";
    m_fontPath[LOGO_BUTTON_START_GAME] = "MainMenu_SelectableButton_StartGame";
    m_fontPath[LOGO_BUTTON_LOAD_GAME] = "MainMenu_SelectableButton_LoadGame";
    m_fontPath[LOGO_BUTTON_TOOL] = "MainMenu_SelectableButton_Tool";
    m_fontPath[LOGO_BUTTON_OPTION] = "MainMenu_SelectableButton_Options";
    m_fontPath[LOGO_BUTTON_CREDITS] = "MainMenu_SelectableButton_Credits";
    m_fontPath[LOGO_BUTTON_QUIT] = "MainMenu_SelectableButton_Quit";


    m_fFontSize[LOGO_BUTTON_CONTINUE_GAME] = _float2(292.f, 30.f);
    m_fFontSize[LOGO_BUTTON_START_GAME] = _float2(222.f, 30.f);
    m_fFontSize[LOGO_BUTTON_LOAD_GAME] = _float2(204.f, 30.f);
    m_fFontSize[LOGO_BUTTON_TOOL] = _float2(100.f, 30.f);

    m_fFontSize[LOGO_BUTTON_OPTION] = _float2(155.f, 30.f);
    m_fFontSize[LOGO_BUTTON_CREDITS] = _float2(137.f, 30.f);
    m_fFontSize[LOGO_BUTTON_QUIT] = _float2(100.f, 30.f);


    UI_DESC desc;
    FaderDesc faderDesc;
    CHUD_Hover::HUDHOVERDESC hoverDesc;

    m_pMainMenuBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_LOGO);
    m_pMainMenuBG.lock()->Set_Depth(0.3f);
    m_pMainMenuBG.lock()->Set_Texture("MainMenu_Background");
    
    m_pLogo = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_LOGO);
    m_pLogo.lock()->Set_Depth(0.2f);
    m_pLogo.lock()->Set_Texture("GameLogo");
    m_pLogo.lock()->Set_UIPosition(((_float)g_iWinCX) * 0.54f, ((_float)g_iWinCY) * 0.2f, 1024.f, 256.f);
    
    
    faderDesc.eFaderType = FADER_IN;
    faderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    faderDesc.fDelayTime = 0.f;
    faderDesc.fFadeMaxTime = 1.f;
    faderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.3f);
    hoverDesc.bSizeChange = false;
    hoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_END;

    m_pLogo.lock()->Init_Fader(faderDesc, hoverDesc);

    faderDesc.eFaderType = FADER_INOUTLOOPING;

    for (auto i = 0; i < (_uint)LOGO_BUTTON_END; i++)
    {
        desc.fX = ((_float)g_iWinCX) * 0.5f;
        desc.fY = ((_float)g_iWinCY) * 0.5f + (55.f * i);
        desc.fSizeX = 776.f;
        desc.fSizeY = 40.f;
        desc.fDepth = 0.1f;

        m_pSelectButton[i] = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_LOGO, &desc);
        m_pSelectButton[i].lock()->Set_Texture("MainMenu_SelectableButton_1");
        m_pSelectButton[i].lock()->Set_Enable(true);
        m_pSelectButton[i].lock()->Init_Fader(faderDesc, hoverDesc);
        desc.fSizeX = m_fFontSize[i].x;
        desc.fSizeY = m_fFontSize[i].y;

        m_pTextTexture[i] = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_LOGO, &desc);
        m_pTextTexture[i].lock()->Set_Texture(m_fontPath[i].c_str());
        m_pTextTexture[i].lock()->Set_Enable(true);
        m_pTextTexture[i].lock()->Set_Depth(0.0f);

        m_pTextTexture[i].lock()->Init_Fader(faderDesc, hoverDesc);

    }

    m_iSelect = 0;
    ChageButtonIndex(m_iSelect);
    m_bSelect = true;

    return S_OK;
}

HRESULT CUI_Logo::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Logo::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
    {
        GAMEINSTANCE->PlaySound2D("UI_ChangeIndex0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));

        if (m_iSelect == 0)
            m_iSelect = LOGO_BUTTON_END - 1;
        else
            m_iSelect--;
        ChageButtonIndex(m_iSelect);
    }
    else if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
    {
        GAMEINSTANCE->PlaySound2D("UI_ChangeIndex0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));

        if (m_iSelect == (_uint)LOGO_BUTTON_QUIT)
            m_iSelect = 0;
        else
            m_iSelect++;
         ChageButtonIndex(m_iSelect);
    }
    if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP) && m_bSelect)
    {
        GAMEINSTANCE->PlaySound2D("UI_Select0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));
        SelectButton(m_iSelect);

    }

}

void CUI_Logo::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    for (auto i = 0; i < (_uint)LOGO_BUTTON_END; i++)
    {
        if (m_iSelect != i)
        {
            m_pSelectButton[i].lock()->Set_AlphaColor(_float4(0.4f, 0.4f, 0.4f, 0.4f));
            m_pTextTexture[i].lock()->Set_AlphaColor(_float4(0.4f, 0.4f, 0.4f, 0.4f));
        }
    }
    if (m_pSelectButton[m_iSelect].lock()->Get_AlphaColor() < 0.4f)
    {
        m_pSelectButton[m_iSelect].lock()->Set_AlphaColor(_float4(0.4f, 0.4f, 0.4f, 0.4f));
        m_pTextTexture[m_iSelect].lock()->Set_AlphaColor(_float4(0.4f, 0.4f, 0.4f, 0.4f));
    }
    if (m_iSelect == (_uint)LOGO_BUTTON_TOOL)
    {
        m_pSelectButton[m_iSelect].lock()->Set_AlphaColor(_float4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pTextTexture[m_iSelect].lock()->Set_AlphaColor(_float4(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

HRESULT CUI_Logo::Render(ID3D11DeviceContext* pDeviceContext)
{


    return S_OK;
}

HRESULT CUI_Logo::SetUp_ShaderResource()
{

    return S_OK;
}

void CUI_Logo::ChageButtonIndex(_uint iButtonIndex)
{
    for (auto i = 0; i < (_uint)LOGO_BUTTON_END; i++)
        m_pSelectButton[i].lock()->Set_Texture("MainMenu_SelectableButton_1");

    m_pSelectButton[iButtonIndex].lock()->Set_Texture("MainMenu_SelectableButton_2");

}

void CUI_Logo::SelectButton(_uint iButtonIndex)
{
    LOGO_BUTTON_TYPE eType = (LOGO_BUTTON_TYPE)iButtonIndex;
    
    string                  szClientSavePath;
    json	                CorvusJson;
    json	                SaveLevelJson;
    LEVEL                   eNextLevel = LEVEL::LEVEL_GAMEPLAY;

    switch (eType)
    {
    case Client::CUI_Logo::LOGO_BUTTON_START_GAME:
        
        szClientSavePath = "../Bin/ClientComponentData/Corvus/SaveData.json";
        CJson_Utility::Save_Json(szClientSavePath.c_str(), CorvusJson);

        Weak_Cast<CLevel_Logo>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(LEVEL::LEVEL_TEST);
        m_bSelect = false;
        break;
    case Client::CUI_Logo::LOGO_BUTTON_CONTINUE_GAME:
        szClientSavePath = "../Bin/SaveLevelData/SaveData.json";

        if (SUCCEEDED(CJson_Utility::Load_Json(szClientSavePath.c_str(), SaveLevelJson)))
        {
            eNextLevel = SaveLevelJson["Level"];
        }
        Weak_Cast<CLevel_Logo>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(eNextLevel);
        m_bSelect = false;
        break;   
    case Client::CUI_Logo::LOGO_BUTTON_LOAD_GAME:
        break;
    case Client::CUI_Logo::LOGO_BUTTON_TOOL:
        Weak_Cast<CLevel_Logo>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(LEVEL::LEVEL_EDIT);
        m_bSelect = false;
        break;
    case Client::CUI_Logo::LOGO_BUTTON_OPTION:
        break;
    case Client::CUI_Logo::LOGO_BUTTON_CREDITS:
        Weak_Cast<CLevel_Logo>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(LEVEL::LEVEL_TEST);
        m_bSelect = false;
        break;
    case Client::CUI_Logo::LOGO_BUTTON_QUIT:
        break;
    case Client::CUI_Logo::LOGO_BUTTON_END:
        break;
    default:
        break;
    }
  
}

void CUI_Logo::Free()
{
    __super::Free();
}
