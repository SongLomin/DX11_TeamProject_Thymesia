#include "stdafx.h"
#include "Player_PotionUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"

GAMECLASS_C(CPlayer_PotionUI)
CLONE_C(CPlayer_PotionUI, CGameObject);

HRESULT CPlayer_PotionUI::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CPlayer_PotionUI::Initialize(void* pArg)
{
    __super::Initialize(pArg);
      
    if (pArg != nullptr)
        memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));
    else
    {
        Set_UIPosition(1260.f, 785.f, 40.f, 40.f);
        m_tUIDesc.fDepth = 0.f;
    }
    m_iMaxPotion = 0;
    m_iCurrentPotion = 0;
    
    m_pFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pFrame.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Frame");
    m_pFrame.lock()->Set_Depth(0.2f);

    m_pFrameBorder = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pFrameBorder.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_FrameBorder");
    m_pFrameBorder.lock()->Set_Depth(0.0f);


    m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pIcon.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Potion_Default_Mini");
    m_pIcon.lock()->Set_Depth(0.0f);


    m_pFontBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pFontBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Font_BG");
    m_pFontBG.lock()->Set_UIPosition(m_tUIDesc.fX + 40.f, m_tUIDesc.fY , 80.f, 28.f);
    m_pFontBG.lock()->Set_Depth(0.1f);

    //left 
   m_tCurrentPotionTextInfo.bAlways = false;
   m_tCurrentPotionTextInfo.bCenterAlign = false;
   m_tCurrentPotionTextInfo.fRotation = 0.f;
   m_tCurrentPotionTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
   m_tCurrentPotionTextInfo.vScale = _float2(0.5, 0.5f);
   m_tCurrentPotionTextInfo.vPosition = _float2(m_tUIDesc.fX + 20.f, m_tUIDesc.fY - 10.f);

   m_tMaxPotionTextInfo.bAlways = false;
   m_tMaxPotionTextInfo.bCenterAlign = false;
   m_tMaxPotionTextInfo.fRotation = 0.f;
   m_tMaxPotionTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
   m_tMaxPotionTextInfo.vScale = _float2(0.5, 0.5f);
   m_tMaxPotionTextInfo.vPosition = _float2(m_tUIDesc.fX + 40.f, m_tUIDesc.fY - 10.f);

    return S_OK;
}

HRESULT CPlayer_PotionUI::Start()
{
    __super::Start();

    return S_OK;
}

void CPlayer_PotionUI::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

#ifdef _DEBUG
    if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
    {
        Set_CurrentPotion(0);

    }
    if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
    {
        Set_CurrentPotion(1);
    }
#endif // _DEBUG


    m_tCurrentPotionTextInfo.szText = to_wstring(m_iCurrentPotion);
    m_tMaxPotionTextInfo.szText = L"/ ";
    m_tMaxPotionTextInfo.szText.append(to_wstring(m_iMaxPotion));
}

void CPlayer_PotionUI::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tCurrentPotionTextInfo);
    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tMaxPotionTextInfo);

}

HRESULT CPlayer_PotionUI::Render()
{
    return S_OK;
}

_uint CPlayer_PotionUI::Get_CrrrentPotion()
{
    return m_iCurrentPotion;
}

void CPlayer_PotionUI::Set_MaxPotion(_uint _iMaxPotion)
{
    m_iMaxPotion = _iMaxPotion;
}

void CPlayer_PotionUI::Set_CurrentPotion(_uint _iCurrentPotion)
{
    m_iCurrentPotion = _iCurrentPotion;

    if (m_iCurrentPotion == 0)
        m_tCurrentPotionTextInfo.vColor = _float4(1.f, 0.f, 0.f, 1.f);
    else
        m_tCurrentPotionTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
}


