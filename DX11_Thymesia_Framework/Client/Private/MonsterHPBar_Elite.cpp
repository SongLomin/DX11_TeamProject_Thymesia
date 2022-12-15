#include "stdafx.h"
#include "MonsterHPBar_Elite.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "HUD_Hover.h"
#include "Fader.h"


GAMECLASS_C(CMonsterHPBar_Elite)
CLONE_C(CMonsterHPBar_Elite, CGameObject)

HRESULT CMonsterHPBar_Elite::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CMonsterHPBar_Elite::Initialize(void* pArg)
{
    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    __super::Initialize(pArg);

    UI_DESC EliteBorderDesc = m_tUIDesc;

    EliteBorderDesc.fSizeX += 50.f;
    EliteBorderDesc.fSizeY += 4.f;
    EliteBorderDesc.fDepth = 0.5f;

    m_pEliteBorder = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &EliteBorderDesc);
    m_pEliteBorder.lock()->Set_Texture("Monster_HPBar_Border_Elite");

    Add_Child(m_pEliteBorder);
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, m_pEliteBorder);
    m_pEliteBorder.lock()->Set_Enable(false);

    Set_Enable(false);
    return S_OK;
}

HRESULT CMonsterHPBar_Elite::Start()
{
    __super::Start();

    

    return S_OK;
}

void CMonsterHPBar_Elite::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CMonsterHPBar_Elite::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CMonsterHPBar_Elite::Render(ID3D11DeviceContext* pDeviceContext)
{
    return S_OK;
}

void CMonsterHPBar_Elite::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);
}

void CMonsterHPBar_Elite::OnDisable()
{
    __super::OnDisable();
}

void CMonsterHPBar_Elite::Set_ChildPosFromThis()
{
    __super::Set_ChildPosFromThis();

    m_pEliteBorder.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
}
