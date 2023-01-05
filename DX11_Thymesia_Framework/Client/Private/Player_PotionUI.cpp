#include "stdafx.h"
#include "Player_PotionUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "Status_Player.h"


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
        //+ 20,20,10,10
        Set_UIPosition(1230.f, 765.f, 50.f, 50.f);
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
    m_pFontBG.lock()->Set_UIPosition(m_tUIDesc.fX + m_tUIDesc.fSizeX, m_tUIDesc.fY , 80.f, 28.f);
    m_pFontBG.lock()->Set_Depth(0.1f);


    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

   Add_Child(m_pIcon);
   Add_Child(m_pFrameBorder);
   Add_Child(m_pFrame);
   Add_Child(m_pFontBG);

    //left 
   m_tCurrentPotionTextInfo.bAlways = false;
   m_tCurrentPotionTextInfo.bCenterAlign = false;
   m_tCurrentPotionTextInfo.fRotation = 0.f;
   m_tCurrentPotionTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
   m_tCurrentPotionTextInfo.vScale = _float2(0.5, 0.5f);
   m_tCurrentPotionTextInfo.vPosition = _float2(m_tUIDesc.fX + 25.f, m_tUIDesc.fY - 10.f);
   m_tCurrentPotionTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

   m_tMaxPotionTextInfo.bAlways = false;
   m_tMaxPotionTextInfo.bCenterAlign = false;
   m_tMaxPotionTextInfo.fRotation = 0.f;
   m_tMaxPotionTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
   m_tMaxPotionTextInfo.vScale = _float2(0.5, 0.5f);
   m_tMaxPotionTextInfo.vPosition = _float2(m_tUIDesc.fX + 45.f, m_tUIDesc.fY - 10.f);
   m_tMaxPotionTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

   GET_SINGLE(CGameManager)->CallBack_ChangePlayer +=
       bind(&CPlayer_PotionUI::Bind_Player, this);

    return S_OK;
}

HRESULT CPlayer_PotionUI::Start()
{
    __super::Start();

    Bind_Player();

    return S_OK;
}

void CPlayer_PotionUI::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

//TODO: UI: 물약 개수 테스트 코드
#ifdef _DEBUG

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

HRESULT CPlayer_PotionUI::Render(ID3D11DeviceContext* pDeviceContext)
{
    return S_OK;
}

void CPlayer_PotionUI::Bind_Player()
{

    weak_ptr<CStatus_Player> pStatus_Player;
    pStatus_Player = GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status();

    pStatus_Player.lock()->Callback_ChangePotion +=
        bind(&CPlayer_PotionUI::Call_ChangePotion, this, placeholders::_1, placeholders::_2);

    Set_MaxPotion(pStatus_Player.lock()->Get_CurrentPotionDesc().m_iMaxPotion);
    Set_CurrentPotion(pStatus_Player.lock()->Get_CurrentPotionDesc().m_iCurrentPotion);

}

void CPlayer_PotionUI::Call_ChangePotion(_uint iCurrentPotion, _uint iMaxPotion)
{
    Set_CurrentPotion(iCurrentPotion);

    Set_MaxPotion(iMaxPotion);
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


