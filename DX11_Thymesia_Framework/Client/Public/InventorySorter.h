#pragma once
#include "Base.h"
#include <DirectXMath.h>

BEGIN(Client)

class CUI_ItemSlot;
class CUI_ShuffleIcon;


class CInventorySorter : public CBase
{
public:
	GAMECLASS_H(CInventorySorter)

public:
	HRESULT				Initialize();

	void				Sorting_Start(vector<weak_ptr<CUI_ItemSlot>> vecItemSlot, _float fItemSlotOffset, _uint iSortType);

public:
	void				Call_OnEndTransformItemSlot();

		
public:
	FDelegate<>			Callback_OnSortingEnd;


private:
	void				SetPostionToSortedReadyItemSlots();
	void				Start_Sort(_uint iSortType);
	void				Start_Lerp();

private:
	vector<weak_ptr<CUI_ItemSlot>> m_vecItemSlot;
	list<weak_ptr<CUI_ShuffleIcon>> m_listShuffleIcon;


	_float			m_fItemSlotOffset;
	_float2			m_fItemSlotStartPos;
	_float2			m_fItemSlotSotringStartPos;

	_float			m_fSortTime;
public:
	static		shared_ptr<CInventorySorter>	Create();


private:
	void				Free();
};

END
