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
	void		Push_Item(ITEM_NAME eItemName);
	_bool		Pop_Item(ITEM_NAME eItemName);
	void		Remove_Item(ITEM_NAME eItemName);
	_uint		Get_Size();

	weak_ptr<CItem> Find_Item(ITEM_NAME eItemName);

	map<ITEM_NAME, shared_ptr<CItem>>	Get_Inventory() { return m_mapInventory; }

private:
	map<ITEM_NAME, shared_ptr<CItem>>	m_mapInventory;

};


END