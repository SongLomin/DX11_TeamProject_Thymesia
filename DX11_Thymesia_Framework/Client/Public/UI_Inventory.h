#pragma once

#include "UI_Container.h"

BEGIN(Client)


class CCustomUI;
class CUI_ItemSlot;


class CUI_Inventory : public CUI_Container
{

public:
    GAMECLASS_H(CUI_Inventory)
    CLONE_H(CUI_Inventory, CGameObject)
public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


private:
    void                    Define_Variable();
    void                    Create_InventoryUI();
    void                    Create_ItemSlot();
    //variable

private:
    void                    Update_KeyInput(_float fTimeDelta);
    void                    Update_ItemSlotOffset();

private:
    int     m_iWidth;
    int     m_iHeight;

private:
    _float2                 m_fItemSlotStart;//Start to LeftTop
    _float                  m_fOffset;


    _float                 m_fScrollOffsetY = 0;
    _float                 m_fScroolSpeed = 30.f;


	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pBG;

private:
    vector<weak_ptr<CUI_ItemSlot>> m_vecItemSlot;

//Free
private:
	void		Free() {};


};

END