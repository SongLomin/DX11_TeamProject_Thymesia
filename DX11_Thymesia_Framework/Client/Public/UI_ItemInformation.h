#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;
class CItem;

class CUI_ItemInformation : public CUI_Container
{
    friend class CPreset_UIDesc;
public:
	GAMECLASS_H(CUI_ItemInformation);
	CLONE_H(CUI_ItemInformation, CGameObject)

public:
    virtual HRESULT Initialize_Prototype()      override;
    virtual HRESULT Initialize(void* pArg)      override;
    virtual HRESULT Start()                     override;
    virtual void Tick(_float fTimeDelta)        override;
    virtual void LateTick(_float fTimeDelta)    override;

private:
    void        Update_Quantity();


public:
    void                            Bind_Item(weak_ptr<CItem> pItem);
    void                            UnBind_Item();

private:
    void                            OnDisable();

    //Variable
private:
    weak_ptr<CItem>                 m_pBindedItem;

private:
    weak_ptr<CCustomUI>              m_pItemTitle;
    weak_ptr<CCustomUI>              m_pItemInformation;
    weak_ptr<CCustomUI>              m_pItemType;
    weak_ptr<CCustomUI>              m_pItemQuantity;
    weak_ptr<CCustomUI>              m_pIcon;

    weak_ptr<CCustomUI>              m_pItemTitleDecoration;
    weak_ptr<CCustomUI>              m_pItemQuantityDecoration;
    weak_ptr<CCustomUI>              m_pItemTypeDecoration;
    
    TEXTINFO                         m_tTextInfoQuantity;

    
private:
    void    Free() { }

};

END

