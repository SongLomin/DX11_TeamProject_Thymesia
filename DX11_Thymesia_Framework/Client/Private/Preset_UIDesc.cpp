#include "stdafx.h"
#include "Preset_UIDesc.h"
#include "UI_MonsterFocus.h"
#include "Player_HPBar.h"
#include "Texture.h"
#include "CustomUI.h"
#include "ProgressBar.h"
#include "UI.h"
#include "HUD_Hover.h"
#include "GameManager.h"
#include "Engine_Defines.h"

void CPreset_UIDesc::Set_CUI_MonsterFocus(weak_ptr<class CUI_MonsterFocus> pUI)
{
	pUI.lock()->Set_UIPosition
	(
		0.f,0.f,100.f,100.f
	);
	pUI.lock()->Set_Depth(0.f);
	pUI.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);
	pUI.lock()->Set_Texture("Target_Icon");

}

void CPreset_UIDesc::Set_CUI_PlayerHPBar(weak_ptr<class CPlayer_HPBar> pUI)
{

    pUI.lock()->Set_UIPosition(250.f, 800.f, 400.f, 15.f);

    CUI::UI_DESC tUIDesc = pUI.lock()->Get_UIDESC();

     pUI.lock()->m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
     pUI.lock()->m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_BG");
     pUI.lock()->m_pBG.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY, tUIDesc.fSizeX, tUIDesc.fSizeY);
     pUI.lock()->m_pBG.lock()->Set_Depth(0.2f);


    pUI.lock()->m_pMainBar = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    pUI.lock()->m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_MainBar");
    pUI.lock()->m_pMainBar.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY, tUIDesc.fSizeX, tUIDesc.fSizeY - 5.f);
    pUI.lock()->m_pMainBar.lock()->Set_Depth(0.1f);

    pUI.lock()->m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    pUI.lock()->m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Left");
    pUI.lock()->m_pBorderLeft.lock()->Set_UIPosition(tUIDesc.fX - (tUIDesc.fSizeX * 0.5f), tUIDesc.fY, 26.f, 15.f);

    pUI.lock()->m_pBorderRight = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    pUI.lock()->m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Right");
    pUI.lock()->m_pBorderRight.lock()->Set_UIPosition(tUIDesc.fX + (tUIDesc.fSizeX * 0.5f), tUIDesc.fY, 26.f, 15.f);


    Engine::FaderDesc tFaderDesc;

    CUI::UI_DESC   tTrackDesc;
    tTrackDesc.fX = tUIDesc.fX + (tUIDesc.fSizeX * 0.5f);
    tTrackDesc.fY = tUIDesc.fY;
    tTrackDesc.fSizeX = 19.f;
    tTrackDesc.fSizeY = 45.f;
    tTrackDesc.fDepth = 0.f;

    tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    tFaderDesc.eFaderType = FADER_TYPE::FADER_INOUTLOOPING;
    tFaderDesc.fDelayTime = 0.f;
    tFaderDesc.fFadeMaxTime = 1.f;
    tFaderDesc.vFadeColor = _float4(0, 0, 0, 1.f);

    CHUD_Hover::HUDHOVERDESC tHoverDesc;
    ZeroMemory(&tHoverDesc, sizeof(CHUD_Hover::HUDHOVERDESC));

    tHoverDesc.bSizeChange = false;
    tHoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA;


    pUI.lock()->m_pTrack = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tTrackDesc);
    pUI.lock()->m_pTrack.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Track");
    pUI.lock()->m_pTrack.lock()->Init_Fader(tFaderDesc, tHoverDesc);
    pUI.lock()->m_pTrack.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

   pUI.lock()-> m_fLerpHp = 0.f;
   pUI.lock()-> m_fCurrentHp = 0.f;
   pUI.lock()-> m_fMaxHp = 0.f;

    pUI.lock()->m_tTextInfo.bAlways = false;
    pUI.lock()->m_tTextInfo.bCenterAlign = false;
    pUI.lock()->m_tTextInfo.fRotation = 0.f;
    pUI.lock()->m_tTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    pUI.lock()->m_tTextInfo.vScale = _float2(0.5, 0.5f);
    pUI.lock()->m_tTextInfo.vPosition = _float2(tUIDesc.fX + tUIDesc.fSizeX * 0.5f + 20.f, tUIDesc.fY - 10.f);
    pUI.lock()->m_tTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    pUI.lock()->m_tUIDesc.fDepth = 0.f;

    pUI.lock()->m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    pUI.lock()->Add_Child(pUI.lock()->m_pMainBar);
    pUI.lock()->Add_Child(pUI.lock()->m_pBG);
    pUI.lock()->Add_Child(pUI.lock()->m_pBorderLeft);
    pUI.lock()->Add_Child(pUI.lock()->m_pBorderRight);

}
