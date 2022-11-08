#include "stdafx.h"
#include "Player_ProgressBar.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"

GAMECLASS_C(CPlayer_ProgressBar)
CLONE_C(CPlayer_ProgressBar, CGameObject);


HRESULT CPlayer_ProgressBar::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CPlayer_ProgressBar::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo(
        TEXT("Shader_HPBar"),
        VTXTEX_DECLARATION::Element,
        VTXTEX_DECLARATION::iNumElements);

    if (pArg != nullptr)
        memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));

    m_fRatio = 1.f;
    
    m_iPassIndex = 9;

    m_eRenderGroup = RENDERGROUP::RENDER_UI;

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_this));

    m_tUIDesc.fDepth = 0.f;

    return S_OK;
}

HRESULT CPlayer_ProgressBar::Start()
{
    __super::Start();

    return S_OK;
}

void CPlayer_ProgressBar::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CPlayer_ProgressBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CPlayer_ProgressBar::Render()
{
    __super::Render();


    return S_OK;
}

void CPlayer_ProgressBar::Set_Ratio(_float _fRatio)
{
    m_fRatio = _fRatio;
}

void CPlayer_ProgressBar::Set_PassIndex(_uint _iPassIndex)
{
    m_iPassIndex = _iPassIndex;
}

void CPlayer_ProgressBar::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);
}

HRESULT CPlayer_ProgressBar::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();


    if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture")))
    {
        MSG_BOX("PlayerHPBar_Error");
    }
    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float));


    return S_OK;
}

void CPlayer_ProgressBar::Free()
{
    __super::Free();
}
