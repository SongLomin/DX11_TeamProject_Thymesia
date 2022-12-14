#pragma once
#include "UI.h"
#include "Status_Player.h"
BEGIN(Client)

#define TAP_STATUS_CNT 10


class CCustomUI;
class CStatus_Player;


class CUI_PauseMenu_Tap_Status :
    public CUI
{
public:
    /*
        
    m_szStatusText.push_back(L"Level");
    m_szStatusText.push_back(L"Attack Damage");
    m_szStatusText.push_back(L"Wound Count");
    m_szStatusText.push_back(L"Claw Damage");
    m_szStatusText.push_back(L"Health Point");
    m_szStatusText.push_back(L"Energy");
    m_szStatusText.push_back(L"Strength");
    m_szStatusText.push_back(L"Vitality");
    m_szStatusText.push_back(L"Plague");
    m_szStatusText.push_back(L"Feather");
    
    */
    enum class STATUSTYPE 
    {
        LEVEL,
        ATTACK_DAMAGE,
        WOUND,
        HP,
        MP,
        STR,
        VIT,
        PLA,
        FEATHER,
        STATUS_END
    };
    GAMECLASS_H(CUI_PauseMenu_Tap_Status)
        CLONE_H(CUI_PauseMenu_Tap_Status, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;


private:
    void                Set_Str(wstring& strStatus, STATUSTYPE  eType);

private:
    void Create_Font();


    virtual void    OnEnable(void* pArg) override; 
    
private:
    weak_ptr< CCustomUI>        m_pStatusBG;
    CStatus_Player::PLAYERDESC  m_tPlayerDesc;
    weak_ptr<CCustomUI>     m_pTextIcon[TAP_STATUS_CNT];
    weak_ptr<CCustomUI>     m_pFontDecoration[TAP_STATUS_CNT];

    TEXTINFO                m_TextInfo[TAP_STATUS_CNT];

    TEXTINFO                m_StatusTextInfo[TAP_STATUS_CNT];

    TEXTINFO                m_CorvusTextInfo;

    vector<wstring>         m_szStatusText;

    weak_ptr<CCustomUI>     m_pUICorvusTextDecoration;
        
};

END

