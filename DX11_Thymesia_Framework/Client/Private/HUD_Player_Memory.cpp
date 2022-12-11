#include "stdafx.h"
#include "HUD_Player_Memory.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"


GAMECLASS_C(CHUD_Player_Memory)
CLONE_C(CHUD_Player_Memory, CGameObject);


HRESULT CHUD_Player_Memory::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CHUD_Player_Memory::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_UIPosition(1400.f, 100.f, 300.f, 30.f);

    m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Player_Memory_BG2");
    m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY);
    m_pBG.lock()->Set_Depth(0.2f);

    m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pIcon.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Player_Memory_Icon");
    m_pIcon.lock()->Set_UIPosition(m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f) + 15.f, m_tUIDesc.fY, 15.f, m_tUIDesc.fSizeY);
    m_pIcon.lock()->Set_Depth(0.1f);

    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    Add_Child(m_pBG);
    Add_Child(m_pIcon);

    m_fMemory = 0.f;
    m_fLerpMemory = 0.f;


    GET_SINGLE(CGameManager)->CallBack_ChangePlayer +=
        bind(&CHUD_Player_Memory::Bind_Player, this);

    return S_OK;
}

HRESULT CHUD_Player_Memory::Start()
{
    __super::Start();

    return S_OK;
}

void CHUD_Player_Memory::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

//TODO: UI: 플레이어 재화 테스트 코드
#ifdef _DEBUG
    if (KEY_INPUT(KEY::J, KEY_STATE::TAP))
    {
        m_fMemory -= 1000.f;
        m_fLerpMemory = m_fMemory;
    }
    if (KEY_INPUT(KEY::K, KEY_STATE::TAP))
    {
        m_fMemory += 1000;
    }
#endif

    if (m_fMemory < 0.f)
    {
        m_fMemory = 0.f;
        m_fLerpMemory = m_fMemory;
    }
    if (m_fMemory != m_fLerpMemory)
    {
        //m_fLerpMemory = SMath::Lerp(m_fLerpMemory, m_fMemory, fTimeDelta * m_fLerpAcc);
        //m_fLerpAcc += (m_fLerpAcc + 1.f);

     //   m_fLerpMemory += m_fLerpAcc;
        
        if (m_fLerpMemory >= m_fMemory || fabsf(m_fLerpMemory - m_fMemory) < 1.f)
        {
            m_fLerpMemory = m_fMemory;
          //  m_fLerpAcc = 1.f;
        }
    }
   
}

void CHUD_Player_Memory::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    TEXTINFO tTextInfo;

    _tchar   szMemoryStr[MAX_PATH] = L"";

    _itow_s((_uint)m_fLerpMemory, szMemoryStr, MAX_PATH, 10);

    tTextInfo.bAlways = false;
    tTextInfo.vPosition = _float2(m_tUIDesc.fX, m_tUIDesc.fY - 5.f);
    tTextInfo.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    tTextInfo.bCenterAlign = false;
    tTextInfo.vScale = _float2(0.4f, 0.4f);
    tTextInfo.szText = szMemoryStr;
    tTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;


    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, tTextInfo);
}

HRESULT CHUD_Player_Memory::Render()
{
  //  __super::Render();


    return S_OK;
}


void CHUD_Player_Memory::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);
}

HRESULT CHUD_Player_Memory::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();


    if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture")))
    {
        MSG_BOX("CHUD_Player_Memory");
    }
    return S_OK;
}


void CHUD_Player_Memory::Call_ChangeMemory(_float fMemory)
{
    
}

void CHUD_Player_Memory::Free()
{
    __super::Free();
}
