#include "stdafx.h"
#include "Player_Memory.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"


GAMECLASS_C(CPlayer_Memory)
CLONE_C(CPlayer_Memory, CGameObject);


HRESULT CPlayer_Memory::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CPlayer_Memory::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_UIPosition(1400.f, 100.f, 300.f, 30.f);

    m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_Memory_BG2");
    m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY);
    m_pBG.lock()->Set_Depth(0.2f);

    m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pIcon.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_Memory_Icon");
    m_pIcon.lock()->Set_UIPosition(m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f) + 15.f, m_tUIDesc.fY, 15.f, m_tUIDesc.fSizeY);
    m_pIcon.lock()->Set_Depth(0.1f);


    m_fMemory = 0.f;
    m_fLerpMemory = 0.f;
    m_fLerpAcc = 1.f;


    return S_OK;
}

HRESULT CPlayer_Memory::Start()
{
    __super::Start();

    return S_OK;
}

void CPlayer_Memory::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (KEY_INPUT(KEY::J, KEY_STATE::TAP))
    {
        m_fMemory -= 1000.f;
        m_fLerpMemory = m_fMemory;
    }
    if (KEY_INPUT(KEY::K, KEY_STATE::TAP))
    {
        m_fMemory += 1000;
    }

    if (m_fMemory < 0.f)
    {
        m_fMemory = 0.f;
        m_fLerpMemory = m_fMemory;
    }
    if (m_fMemory != m_fLerpMemory)
    {
        //m_fLerpMemory = SMath::Lerp(m_fLerpMemory, m_fMemory, fTimeDelta * m_fLerpAcc);
        m_fLerpAcc += (m_fLerpAcc + 1.f);

        m_fLerpMemory += m_fLerpAcc;
        
        if (m_fLerpMemory >= m_fMemory || fabsf(m_fLerpMemory - m_fMemory) < 1.f)
        {
            m_fLerpMemory = m_fMemory;
            m_fLerpAcc = 1.f;
        }
    }
   
}

void CPlayer_Memory::LateTick(_float fTimeDelta)
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
    

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, tTextInfo);
}

HRESULT CPlayer_Memory::Render()
{
  //  __super::Render();


    return S_OK;
}


void CPlayer_Memory::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);
}

HRESULT CPlayer_Memory::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();


    if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture")))
    {
        MSG_BOX("CPlayer_Memory");
    }
    return S_OK;
}

void CPlayer_Memory::Free()
{
    __super::Free();
}
