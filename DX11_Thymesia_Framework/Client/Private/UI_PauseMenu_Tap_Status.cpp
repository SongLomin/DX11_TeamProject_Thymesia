#include "stdafx.h"
#include "UI_PauseMenu_Tap_Status.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"


GAMECLASS_C(CUI_PauseMenu_Tap_Status)
CLONE_C(CUI_PauseMenu_Tap_Status, CGameObject)

HRESULT CUI_PauseMenu_Tap_Status::Initialize_Prototype()
{
    __super::Initialize_Prototype();


    return S_OK;
}

HRESULT CUI_PauseMenu_Tap_Status::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    return S_OK;
}

HRESULT CUI_PauseMenu_Tap_Status::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_PauseMenu_Tap_Status::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CUI_PauseMenu_Tap_Status::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_PauseMenu_Tap_Status::Render()
{
    //
    return S_OK;
}
