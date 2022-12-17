#include "stdafx.h"
#include "UI_Cursor.h"
#include "UI_Utils.h"

GAMECLASS_C(CUI_Cursor)
CLONE_C(CUI_Cursor, CGameObject)

HRESULT CUI_Cursor::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

    CreateCursor();

    Set_Enable(false);
    return S_OK;
}

HRESULT CUI_Cursor::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Cursor::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::Tick(fTimeDelta);

    POINT		pt;

    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);


    Set_UIPosition(pt.x + (m_fCursorSize * 0.5f), pt.y + (m_fCursorSize * 0.5f),
        m_fCursorSize, m_fCursorSize);
    


}

void CUI_Cursor::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);


}

void CUI_Cursor::CreateCursor()
{
    m_tMousePt = { 0,0 };

    Set_UIPosition(0.f, 0.f, m_fCursorSize, m_fCursorSize);
    Set_Texture("Cursor");

}

void CUI_Cursor::Free()
{
}
