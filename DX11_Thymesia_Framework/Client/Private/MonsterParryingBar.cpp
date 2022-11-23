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

    m_tUIDesc.fSizeX = 170.f;
    m_tUIDesc.fSizeY = 10.f;
    m_tUIDesc.fX = g_iWinCX / 2.f;
    m_tUIDesc.fY = g_iWinCY / 2.f;
    m_tUIDesc.fDepth = 0.1f;



   m_pBorder = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
   m_pBorder.lock()->Set_Texture("Monster_HPBar_Border");

   UI_DESC tMainBarDesc = m_tUIDesc;
   m_tUIDesc.fSizeX = 0.f;
   m_tUIDesc.fY = m_tUIDesc.fSizeY - 4.f;


    m_pMainBar = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tMainBarDesc);
    m_pMainBar.lock()->Set_Texture("Monster_Parry_Fill");
    m_pMainBar.lock()->Set_Depth(0.0f);

    m_fCrurrentParryGauge = 0.f;
    m_fLerpedParryGauge = 0.f;


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


    if (KEY_INPUT(KEY::Z, KEY_STATE::TAP))
    {
        m_fCrurrentParryGauge -= 50.f;

        if (m_fCrurrentParryGauge <= 0)
            m_fCrurrentParryGauge = 0.f;
     //   else
          //  Set_Lerp(m_fLerpedParryGauge, m_fCrurrentParryGauge, 1.f, EASING_TYPE::QUAD_IN);
    }
    else if (KEY_INPUT(KEY::X, KEY_STATE::TAP))
    {
        m_fCrurrentParryGauge += 50.f;
     
        if (m_fCrurrentParryGauge >= 100.f)
            m_fCrurrentParryGauge = 100.f;
        //else
        //    Set_Lerp(m_fLerpedParryGauge, m_fCrurrentParryGauge, 1.f,EASING_TYPE::QUAD_IN);

    }
   // if (Is_Lerping())
    //{
    //  m_fLerpedParryGauge = Get_Lerp().x;

       //_float fSizeX = m_tUIDesc.fSizeX * (m_fLerpedParryGauge * 0.01f);
      m_pMainBar.lock()->Set_SizeX(m_tUIDesc.fSizeX *  (m_fCrurrentParryGauge / 100.f));
    //}
 

}

void CMonsterParryingBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CMonsterParryingBar::Render()
{
    //

    return S_OK;
}
