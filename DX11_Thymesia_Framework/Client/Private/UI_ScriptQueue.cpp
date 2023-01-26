#include "stdafx.h"
#include "UI_ScriptQueue.h"
#include "UI_Containers.h"
#include "UI_Elements.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"
#include "State_Player.h"
#include "Player.h"

#include "UI_Script.h"
#include "GameManager.h"


GAMECLASS_C(CUI_ScriptQueue)
CLONE_C(CUI_ScriptQueue, CGameObject)

HRESULT CUI_ScriptQueue::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pScript = GAMEINSTANCE->Add_GameObject<CUI_Script>(LEVEL_STATIC);
      
    return S_OK;
}

HRESULT CUI_ScriptQueue::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_ScriptQueue::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_pScript.lock()->Get_Enable() == false)
    {
        if (!m_ScriptDescQueue.empty())
        {
            m_pScript.lock()->ReadyScript(m_ScriptDescQueue.front());
            m_ScriptDescQueue.pop();
        }
    }
}

void CUI_ScriptQueue::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CUI_ScriptQueue::Call_SetScript_Tutorial_Varg()
{

    CUI_Script::SCRIPTDESC tScriptDesc;

    tScriptDesc.fLifeTime = 3.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, g_iWinCY / 2.f);
    tScriptDesc.fSize = _float2(g_iWinCX, g_iWinCY);
    tScriptDesc.strScriptKey = "Script_Varg_Tutorial_Appear";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);

    tScriptDesc.fLifeTime = 2.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, 850.f);
    tScriptDesc.fSize = _float2(844.f, 43.f);
    tScriptDesc.strScriptKey = "Script_Varg_Tutorial_0";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);

    tScriptDesc.fLifeTime = 1.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, 850.f);
    tScriptDesc.fSize = _float2(410.f, 36.f);
    tScriptDesc.strScriptKey = "Script_Varg_Tutorial_1";
    m_ScriptDescQueue.push(tScriptDesc);
}

void CUI_ScriptQueue::Call_SetScript_Tutorial_Varg_Appear()
{
    CUI_Script::SCRIPTDESC tScriptDesc;

    tScriptDesc.fLifeTime = 1.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, g_iWinCY / 2.f);
    tScriptDesc.fSize = _float2(g_iWinCX, g_iWinCY);
    tScriptDesc.strScriptKey = "Script_Varg_Tutorial_Appear";
    tScriptDesc.bLerp = false;
    m_ScriptDescQueue.push(tScriptDesc);
}

void CUI_ScriptQueue::Call_SetScript_Varg_Page2()
{
}

void CUI_ScriptQueue::Call_SetScript_Varg_Dead()
{
}

void CUI_ScriptQueue::Call_SetScript_Bat_Appear()
{
    CUI_Script::SCRIPTDESC tScriptDesc;

    tScriptDesc.fLifeTime = 3.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, g_iWinCY / 2.f);
    tScriptDesc.fSize = _float2(g_iWinCX, g_iWinCY);
    tScriptDesc.strScriptKey = "Script_Bat_Appear";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);
}

void CUI_ScriptQueue::Call_SetScript_Urd_Appear()
{
    CUI_Script::SCRIPTDESC tScriptDesc;

    tScriptDesc.fLifeTime = 3.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, g_iWinCY / 2.f);
    tScriptDesc.fSize = _float2(g_iWinCX, g_iWinCY);
    tScriptDesc.strScriptKey = "Script_Urd_Appear";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);

    tScriptDesc.fLifeTime = 2.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, 850.f);
    tScriptDesc.fSize = _float2(844.f, 43.f);
    tScriptDesc.strScriptKey = "Script_Urd_BattleStart0";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);

    tScriptDesc.fLifeTime = 1.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, 850.f);
    tScriptDesc.fSize = _float2(844.f, 43.f);
    tScriptDesc.strScriptKey = "Script_Urd_BattleStart1";
    m_ScriptDescQueue.push(tScriptDesc);
}

void CUI_ScriptQueue::Call_SetScript_Urd_Phase2()
{
    CUI_Script::SCRIPTDESC tScriptDesc;

    tScriptDesc.fLifeTime = 2.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, 850.f);
    tScriptDesc.fSize = _float2(844.f, 43.f);
    tScriptDesc.strScriptKey = "Script_Urd_Phase2";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);
}

void CUI_ScriptQueue::Call_SetScript_Urd_Dead()
{
    CUI_Script::SCRIPTDESC tScriptDesc;

    tScriptDesc.fLifeTime = 2.f;
    tScriptDesc.fPos = _float2(g_iWinCX / 2.f, 850.f);
    tScriptDesc.fSize = _float2(844.f, 43.f);
    tScriptDesc.strScriptKey = "Script_Urd_Dead";
    tScriptDesc.bLerp = true;
    m_ScriptDescQueue.push(tScriptDesc);
}

void CUI_ScriptQueue::OnEnable(void* pArg)
{
    if (m_pScript.lock()->GetScriptPlayingType() != CUI_Script::SCRIPTPLAYINGTYPE::SCRIPT_END)
    {
        m_pScript.lock()->Set_Enable(true);
    }
}

void CUI_ScriptQueue::OnDisable()
{
    m_pScript.lock()->Set_Enable(false);
}

void CUI_ScriptQueue::Free()
{
}
