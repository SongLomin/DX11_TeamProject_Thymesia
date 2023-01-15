#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CItem : public CBase
{
	friend class CPreset_ItemData;
	friend class CInventory;
public:
	GAMECLASS_H(CItem)

public:
	virtual HRESULT	Initalize(ITEM_NAME eItemName);

public:
	ITEM_NAME				Get_Name() { return m_eItemName; }
	ITEM_TYPE				Get_Type() { return m_eItemType; }
	_int					Get_CurrentQuantity() { return m_iCurrentQuantity; }
	_int					Get_MaxQuantity() { return m_iMaxQuantity; }
	time_t					Get_CreatedTime() { return m_iCreatedTime; }

private:
	void			Set_CurrentQuantity(_int iQuantity);//인벤토리만
	void			Set_CreatedTime(time_t iCreatedTime);

public:
	void			Add_Quantity();
	void			Minus_Quantity();
	void			Minus_Quantity(_uint iQuantity);


//varriable
private:
	ITEM_NAME		m_eItemName;
	ITEM_TYPE		m_eItemType;
	_int			m_iMaxQuantity;
	_int			m_iCurrentQuantity;

	time_t			m_iCreatedTime;//만들어진 시간

public:
	static shared_ptr<CItem>	Create(ITEM_NAME eItemName);
	
private:
	void		Free() {};

};
END
