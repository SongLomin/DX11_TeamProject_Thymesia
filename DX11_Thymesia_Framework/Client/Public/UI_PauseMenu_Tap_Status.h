#pragma once
#include "UI.h"

BEGIN(Client)

#define TAP_STATUS_CNT 10


class CCustomUI;

class CUI_PauseMenu_Tap_Status :
    public CUI
{
public:
    GAMECLASS_H(CUI_PauseMenu_Tap_Status)
        CLONE_H(CUI_PauseMenu_Tap_Status, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    void Create_Font();


private:
    weak_ptr< CCustomUI> m_pStatusBG;
    
    weak_ptr<CCustomUI>     m_pTextIcon[TAP_STATUS_CNT];
    weak_ptr<CCustomUI>     m_pFontDecoration[TAP_STATUS_CNT];

    TEXTINFO                m_TextInfo[TAP_STATUS_CNT];

    TEXTINFO                m_StatusTextInfo[TAP_STATUS_CNT];

    TEXTINFO                m_CorvusTextInfo;

    vector<wstring>         m_szStatusText;

    weak_ptr<CCustomUI>     m_pUICorvusTextDecoration;
        
};

END

