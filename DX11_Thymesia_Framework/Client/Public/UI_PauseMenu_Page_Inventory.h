#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;
class CUI_Inventory;
class CUI_ItemInformation;
class CItem;



class CUI_PauseMenu_Page_Inventory : public CUI_Container
{
//Funciton
public:
	GAMECLASS_H(CUI_PauseMenu_Page_Inventory)
	CLONE_H(CUI_PauseMenu_Page_Inventory, CGameObject)

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

private:
    void            Create_InventoryUITap();
    void            Create_ItemInformationTap();
    virtual void    OnEnable(void* pArg);
    virtual void    OnDisable();
    

private:
    void            Free() {};
//Variable

private:
    void            Call_OnMouseOver(weak_ptr<CItem> pItem);
    void            Call_OnMouseOut();


private://
    weak_ptr<CUI_Inventory>             m_pInventory;
    weak_ptr<CUI_ItemInformation>       m_pItemInformation;


};


END
