#include "stdafx.h"
#include "MonsterParryingBar.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"

#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "HUD_Hover.h"
#include "Fader.h"
#include "PipeLine.h"
#include "Monster.h"
#include "Status_Monster.h"
#include "UI_LerpBar.h"



GAMECLASS_C(CMonsterParryingBar)
CLONE_C(CMonsterParryingBar, CGameObject)


HRESULT CMonsterParryingBar::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonsterParryingBar::Initialize(void* pArg)
{
    __super::Initialize(pArg);

  // m_pBorder = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
   //m_pBorder.lock()->Set_Texture("Monster_HPBar_Border");
   //m_pBorder.lock()->Set_Depth(0.1f);

   UI_DESC tMainBarDesc = m_tUIDesc;

   tMainBarDesc.fSizeX = m_tUIDesc.fSizeX + 20.f;
   tMainBarDesc.fSizeY = m_tUIDesc.fSizeY - 4.f;
   tMainBarDesc.fDepth = 0.0f;

    m_pMainBar = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &tMainBarDesc);
    m_pMainBar.lock()->Set_Texture("Monster_Parry_Fill");
    m_pMainBar.lock()->Set_PassIndex(7);
    m_pMainBar.lock()->Set_Ratio(0.f);

    m_fCrurrentParryGauge = 0.f;
    m_fLerpedParryGauge = 0.f;

    //Add_Child(m_pBorder);
    Add_Child(m_pMainBar);

    return S_OK;
}

HRESULT CMonsterParryingBar::Start()
{
    __super::Start();

    return S_OK;
}

void CMonsterParryingBar::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (Is_Lerping())
    {
      m_fLerpedParryGauge = Get_Lerp().x;

      m_pMainBar.lock()->Set_Ratio(m_fLerpedParryGauge);
    }
 
}

void CMonsterParryingBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CMonsterParryingBar::Render(ID3D11DeviceContext* pDeviceContext)
{

    return S_OK;
}

void CMonsterParryingBar::Reset()
{
}

void CMonsterParryingBar::Set_UIPosition(const _float fX, const _float fY)
{
    __super::Set_UIPosition(fX, fY);

   // m_pBorder.lock()->Set_UIPosition(fX, fY);
    m_pMainBar.lock()->Set_UIPosition(fX, fY);

}

void CMonsterParryingBar::Set_Ratio(_float fRatio, _bool bRatio)
{
    m_fCrurrentParryGauge = fRatio;

    if (bRatio)
        Set_Lerp(m_fLerpedParryGauge, m_fCrurrentParryGauge, 1.f, EASING_TYPE::EXPO_OUT);
    else
    {
        m_fLerpedParryGauge = m_fCrurrentParryGauge;
        m_pMainBar.lock()->Set_Ratio(m_fLerpedParryGauge);
    }
}
