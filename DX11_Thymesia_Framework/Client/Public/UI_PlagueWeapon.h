#pragma once
#include "CustomUI.h"


BEGIN(Client)

class CUI_PlagueWeapon : public CCustomUI
{
public:
    GAMECLASS_H(CUI_PlagueWeapon)
        CLONE_H(CUI_PlagueWeapon, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void                Set_MainText(const _tchar* pKey);
    void                Set_SkillInfo(const char*   pKey, const _tchar* pSkillName);
    void                Set_SkillIcon(SKILL_NAME eSkillName);
    void                Set_IconSize(_float fSizeX, _float fSizeY);

private:
    TEXTINFO        m_MainTextInfo;
    TEXTINFO        m_WeaponTextInfo;
    weak_ptr<CCustomUI> m_pIcon;
    weak_ptr<CCustomUI> m_pDecoration;


private:




	



};









END

