#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CItem;

class CInventory : public CClientComponent
{
public:
	GAMECLASS_H(CInventory)
		CLONE_H(CInventory, CComponent)
		SHALLOW_COPY(CInventory)

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	void		Push_Item(ITEM_NAME eItemName, _bool bCallPopup = true);
	void		Push_Item(MONSTERTYPE eMonsterType);

	_bool		Pop_Item(ITEM_NAME eItemName);
	void		Remove_Item(ITEM_NAME eItemName);
	_uint		Get_Size();

	void		Use_Item(ITEM_NAME eItemName,  _uint iUseQuantity = 1);
	weak_ptr<CItem> Find_Item(ITEM_NAME eItemName);

	map<ITEM_NAME, shared_ptr<CItem>>	Get_Inventory() { return m_mapInventory; }


public:
	virtual void    Write_SaveData(json& Out_Json) override;
	virtual void    Load_SaveData(const json& In_Json) override;

private:
	typedef map<ITEM_NAME, shared_ptr<CItem>> ITEMMAP;
	map<ITEM_NAME, shared_ptr<CItem>>	m_mapInventory;

};


END