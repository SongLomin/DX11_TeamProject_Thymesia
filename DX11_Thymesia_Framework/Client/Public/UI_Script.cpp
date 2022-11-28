#include "stdafx.h"
#include "UI_Script.h"
#include "UI_Elements.h"
#include "EasingTransform.h"

GAMECLASS_C(CUI_Script)
CLONE_C(CUI_Script, CGameObject)


HRESULT CUI_Script::Initialize(void* pArg)
{
    m_pEasingTrnaformCom = Add_Component<CEasingTransform>();

    return S_OK;
}

void CUI_Script::Tick(_float fTimeDelta)
{
    if (m_pScript.lock()->Get_Enable())//스크립트가 떠있다면(돌고 있다면)
    {

    }
    else if (!m_pEasingTrnaformCom.lock()->Is_Lerping())
    {
        //러프도 끝났고, 다음 스크립트도 없다면
        if (m_ScriptQueue.empty())
            m_pScript.lock()->Set_Enable(false);//스크립트 안보이게.
    }
    
}

void CUI_Script::LateTick(_float fTimeDelta)
{

}

void CUI_Script::Excute()
{
    

}

void CUI_Script::Add_Script(SCRIPTDESC script)
{
    m_ScriptQueue.push(script);
}
