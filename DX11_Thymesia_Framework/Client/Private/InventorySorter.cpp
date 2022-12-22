#include "stdafx.h"
#include "InventorySorter.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "UI_ItemSlot.h"
#include "Item.h"
#include "UI_ShuffleIcon.h"
#include "UI_Inventory.h"
#include "CUtils_EasingTransform.h"
#include "EasingComponent_Transform.h"
GAMECLASS_C(CInventorySorter)


HRESULT CInventorySorter::Initialize()
{
	m_fItemSlotSotringStartPos = { 148.f, 78.f };//Left Top


	return S_OK;
}

void CInventorySorter::Sorting_Start(vector<weak_ptr<CUI_ItemSlot>> vecItemSlot, _float fItemSlotOffset, _uint iSortType)
{
	//소팅 시작.

	m_vecItemSlot = vecItemSlot;

	m_fItemSlotOffset = fItemSlotOffset;

	SetPostionToSortedReadyItemSlots();
    Start_Sort(iSortType);

    m_fSortTime = 0.6f;

    Start_Lerp();
}


void CInventorySorter::Call_OnEndTransformItemSlot()//뽑혀서 정렬되는 아이템 슬롯이 끝났을 때
{


}

void CInventorySorter::SetPostionToSortedReadyItemSlots()//정렬해야되는 아이템슬롯들 위로 올림
{
	weak_ptr<CUI_ShuffleIcon> pShuffleIcon;

	for(_uint i = 0 ; i < m_vecItemSlot.size();i++)
	{
		pShuffleIcon = GAMEINSTANCE->Add_GameObject<CUI_ShuffleIcon>(LEVEL_STATIC);

		pShuffleIcon.lock()->Set_UIPosition(m_fItemSlotSotringStartPos.x +  i * m_fItemSlotOffset,
			m_fItemSlotSotringStartPos.y, 64.f, 64.f, CUI::ALIGN_LEFTTOP);
	
        pShuffleIcon.lock()->Bind_Item(m_vecItemSlot[i].lock()->Get_BindItem());
		m_listShuffleIcon.push_back(pShuffleIcon);
	}
}

void CInventorySorter::Start_Sort(_uint iSortType)
{
	CUI_Inventory::INVENTORY_SORTTYPE eSortType = (CUI_Inventory::INVENTORY_SORTTYPE)iSortType;

	sort(m_vecItemSlot.begin(), m_vecItemSlot.end(),
    [&](weak_ptr<CUI_ItemSlot> pFirst, weak_ptr<CUI_ItemSlot> pSecond)
    {
        switch (eSortType)
        {
        case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_DATE:
        {
            if (pFirst.lock()->Get_BindItem().lock()->Get_CreatedTime() <
                pSecond.lock()->Get_BindItem().lock()->Get_CreatedTime())
            {
            //    _float2 fPosDest = pFirst.lock()->GetPos();
           //     _float2 fPosSour = pSecond.lock()->GetPos();

           //     pFirst.lock()->Set_UIPosition(fPosSour.x, fPosSour.y);
           //     pSecond.lock()->Set_UIPosition(fPosDest.x, fPosDest.y);
   
                return true;
            }
        }
        break;
        case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_TYPE:
            if (pFirst.lock()->Get_BindItem().lock()->Get_Type() <
                pSecond.lock()->Get_BindItem().lock()->Get_Type())
            {
            //    _float2 fPosDest = pFirst.lock()->GetPos();
           //     _float2 fPosSour = pSecond.lock()->GetPos();

            //    pFirst.lock()->Set_UIPosition(fPosSour.x, fPosSour.y);
            //    pSecond.lock()->Set_UIPosition(fPosDest.x, fPosDest.y);


                return true;

            }
            break;
        case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_QUANTITY:
            if (pFirst.lock()->Get_BindItem().lock()->Get_CurrentQuantity() >
                pSecond.lock()->Get_BindItem().lock()->Get_CurrentQuantity())
            {
            //    _float2 fPosDest = pFirst.lock()->GetPos();
            //   _float2 fPosSour = pSecond.lock()->GetPos();
            //  pFirst.lock()->Set_UIPosition(fPosSour.x, fPosSour.y);
            //   pSecond.lock()->Set_UIPosition(fPosDest.x, fPosDest.y);


                return true;
            }
            break;
        default:
            break;
        }
         return false;
    });
    list<weak_ptr<CUI_ShuffleIcon>>::iterator iter = m_listShuffleIcon.begin();
    //소팅이 끝나면 아이템 언바인드.
    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->UnBind_Item();
        iter->lock()->Bind_Target(elem);
        ++iter;
    }
}

void CInventorySorter::Start_Lerp()
{
    if (m_listShuffleIcon.empty())
        return;

    weak_ptr<CUI_ShuffleIcon> pIcon = m_listShuffleIcon.front();

    m_listShuffleIcon.erase(m_listShuffleIcon.begin());

    pIcon.lock()->Get_Component<CEasingComponent_Transform>().lock()->Callback_LerpEnd
        += bind(&CInventorySorter::Start_Lerp, this);
        
    pIcon.lock()->Start_Lerp(m_fSortTime);

    for (auto& elem : m_listShuffleIcon)//한칸씩 앞으로 땡긴다.
    {
        elem.lock()->Pull_Lerp();
    }

}

shared_ptr<CInventorySorter> CInventorySorter::Create()
{
	shared_ptr<CInventorySorter> pInstance = make_shared<CInventorySorter>();

	pInstance->Initialize();

	return pInstance;
}


void CInventorySorter::Free()
{
		
}
