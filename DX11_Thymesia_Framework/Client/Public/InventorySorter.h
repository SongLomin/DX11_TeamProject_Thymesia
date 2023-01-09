#pragma once
#include "Base.h"
#include <DirectXMath.h>

BEGIN(Client)

class CUI_ItemSlot;
class CUI_ShuffleIcon;
class CItem;

class CInventorySorter : public CBase
{
public:
	GAMECLASS_H(CInventorySorter)

public:
	enum class SORTING_ANIMATION_TYPE
	{
		SORTING_ANIMATION_MOBILE,//모바일 갬성 정렬
		SORTING_ANIMATION_QUICK,//
		SORTING_ANIMATION_QUICK_FLOW_BEZIER//퀵소트 플로우 보여주면서(베지어 곡선추가)
	};

public:
	HRESULT				Initialize();

	vector<weak_ptr<CUI_ItemSlot>>			Sorting_Start(vector<weak_ptr<CUI_ItemSlot>>& vecItemSlot, _float fItemSlotOffset, _uint iSortType,
		SORTING_ANIMATION_TYPE eAnimType = SORTING_ANIMATION_TYPE::SORTING_ANIMATION_MOBILE);

public:
	void				Call_OnEndTransformItemSlot();

		
public:
	FDelegate<>			Callback_OnSortingEnd;


//Start					AnimaitonType_Mobile
private:
	void				SetPostionToSortedReadyItemSlots();
	void				Start_Sort_TypeMobile(_uint iSortType);
	void				Start_Animation_TypeMobile();

private:
	vector<weak_ptr<CUI_ItemSlot>> m_vecItemSlot;
	list<weak_ptr<CUI_ShuffleIcon>> m_listShuffleIcon;


	_float			m_fItemSlotOffset;
	_float2			m_fItemSlotStartPos;
	_float2			m_fItemSlotSotringStartPos;

	_float			m_fSortTime;

	_uint			m_iSortType;

/*
	아이템의 현재 슬롯.
	아이템이 가야할 슬롯.

	섞여야할 슬롯의 아이템.
*/

private:
	void		Make_ShuffleIcon();

	void		MyQuickSort(vector<weak_ptr<CUI_ItemSlot>>& vecItemSlots, _int iLeft, _int iRight);
	_int		MyPartition(vector<weak_ptr<CUI_ItemSlot>>& vecItemSlots, _int iLeft, _int iRight,
		_uint iSortType);

	void		MySwap(vector<weak_ptr<CUI_ItemSlot>>& vecItemSlots, _int iLeft, _int iRight);

	void		Start_Animation_TypeFlow();

	//모든 정렬은 내림차순 기준으로
	_bool		CheckCondition(weak_ptr<CItem>	pLeftItem, weak_ptr<CItem>	pRightItem, _uint iSortType);


private:
	list<_int>		m_SortFlowList;
	vector<weak_ptr<CUI_ShuffleIcon>> m_vecShuffleIcon;
	

public:
	static		shared_ptr<CInventorySorter>	Create();


private:
	void				Free();
};

END
