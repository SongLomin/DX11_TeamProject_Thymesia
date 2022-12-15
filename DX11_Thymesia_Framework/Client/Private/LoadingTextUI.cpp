#include "stdafx.h"
#include "LoadingTextUI.h"
#include "CustomUI.h"
#include "DissolveUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "Shader.h"
#include "Fader.h"
GAMECLASS_C(CLoadingTextUI);
CLONE_C(CLoadingTextUI, CGameObject);

HRESULT CLoadingTextUI::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CLoadingTextUI::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pMask = Add_Component<CTexture>();

    m_pMask.lock()->Use_Texture("Mask_Horizontal");

    m_iPassIndex = 5;

    m_bDelay = false;
    return S_OK;
}

HRESULT CLoadingTextUI::Start()
{
    __super::Start();

    return S_OK;
}

void CLoadingTextUI::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_bDelay)
    {
        if ((m_fStartDelay -= fTimeDelta) < 0.f)
        {
            m_fTick += fTimeDelta;
            m_fRatio = (m_fTick / m_fTime);
            if (m_fTick >= m_fTime)
            {
                m_bDelay = false;
            }
        }
    }
}

void CLoadingTextUI::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CLoadingTextUI::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (m_fRatio >= 0.01f)
        __super::Render(pDeviceContext);

    return S_OK;
}

HRESULT CLoadingTextUI::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pMask.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture");

    return S_OK;
}

void CLoadingTextUI::Set_DelayAlpha(_float fDelayTime, _float fStartTime)
{
    m_fStartDelay = fDelayTime;
    m_fTime = fStartTime;
    m_fTick = 0.f;
    m_fRatio = 0.f;
    m_bDelay = true;
}

void CLoadingTextUI::Free()
{
    __super::Free();
}
