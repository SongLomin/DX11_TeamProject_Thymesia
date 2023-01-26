#include "stdafx.h"
#include "ProgressBar.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"

GAMECLASS_C(CProgressBar)
CLONE_C(CProgressBar, CGameObject);


HRESULT CProgressBar::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CProgressBar::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_fRatio = 1.f;
    
    m_iPassIndex = 2;


    return S_OK;
}

HRESULT CProgressBar::Start()
{
    __super::Start();

    return S_OK;
}

void CProgressBar::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CProgressBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CProgressBar::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);


    return S_OK;
}

void CProgressBar::Set_Ratio(_float _fRatio)
{
    m_fRatio = _fRatio;
}

void CProgressBar::Set_PassIndex(_uint _iPassIndex)
{
    m_iPassIndex = _iPassIndex;
}

void CProgressBar::Decrease_Ratio(_float fRatio)
{
    m_fRatio -= fRatio;

    if (m_fRatio <= 0.f)
        m_fRatio = 0.f;
}

void CProgressBar::Increase_Ratio(_float fRatio)
{
    m_fRatio += fRatio;
    if (m_fRatio >= 1.f)
        m_fRatio = 1.f;
}

void CProgressBar::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);
}

HRESULT CProgressBar::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float));

    return S_OK;
}

void CProgressBar::Free()
{
    __super::Free();
}
