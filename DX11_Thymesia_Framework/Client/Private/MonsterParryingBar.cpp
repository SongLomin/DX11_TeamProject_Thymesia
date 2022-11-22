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

HRESULT CMonsterParryingBar::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonsterParryingBar::Initialize(void* pArg)
{
    __super::Initialize(pArg);


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
