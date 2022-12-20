#include "stdafx.h"
#include "ItemPopup_Queue.h"
#include "CustomUI.h"
#include "UI_ItemPopup.h"

void CItemPopup_Queue::AddPopup(ITEM_NAME eItemName)
{
	for (auto& elem : m_pItemPopupList)
	{
		elem.lock()->Add_Y(m_fPopupOffset);
	}
	Create_PopupUI(eItemName);
}

void CItemPopup_Queue::Call_EndFadeOut(weak_ptr<CUI_ItemPopup> pItemPopup)
{
	for (auto iter = m_pItemPopupList.begin(); iter != m_pItemPopupList.end();)
	{
		if (iter->lock() == pItemPopup.lock())
		{
			m_pItemPopupList.erase(iter);
			return;
		}
		++iter;
	}
}

void CItemPopup_Queue::Create_PopupUI(ITEM_NAME eItemName)
{
	weak_ptr<CUI_ItemPopup>	pItemPopup = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CUI_ItemPopup>(LEVEL_STATIC);

	if (!pItemPopup.lock())
	{
		pItemPopup = GAMEINSTANCE->Add_GameObject<CUI_ItemPopup>(LEVEL_STATIC);
	}
	pItemPopup.lock()->Ready_Popup(eItemName);
	pItemPopup.lock()->Start_Popup();

	pItemPopup.lock()->Callback_EndFadeOut += 
		bind(&CItemPopup_Queue::Call_EndFadeOut, this, placeholders::_1);

	m_pItemPopupList.push_back(pItemPopup);
}

shared_ptr<CItemPopup_Queue> CItemPopup_Queue::Create()
{
	shared_ptr<CItemPopup_Queue> pItemPopupQueue = make_shared<CItemPopup_Queue>();

	pItemPopupQueue->Set_ThisInstance(pItemPopupQueue);

	return pItemPopupQueue;
}
