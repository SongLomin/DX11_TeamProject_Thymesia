#include "stdafx.h"
#include "ItemPopup_Queue.h"
#include "CustomUI.h"
#include "UI_ItemPopup.h"

void CItemPopup_Queue::AddPopup(ITEM_NAME eItemName)
{
	Create_PopupUI(eItemName);

	Pull_Popup();
}

void CItemPopup_Queue::Call_EndFadeOut(weak_ptr<CUI_ItemPopup> pItemPopup)
{
	for (auto iter = m_pItemPopupList.begin(); iter != m_pItemPopupList.end();)
	{
		if (iter->lock() == pItemPopup.lock())
		{
			m_pItemPopupList.erase(iter);
			Pull_Popup();
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
	else
	{
		pItemPopup.lock()->Reset();
	}
	pItemPopup.lock()->Ready_Popup(eItemName);

	
	m_pItemPopupQueue.push(pItemPopup);
}

void CItemPopup_Queue::Pull_Popup()
{
	if (m_pItemPopupList.size() >= m_iMaxViewPopup)
		return;

	if (m_pItemPopupQueue.empty())
		return;

	for (auto& elem : m_pItemPopupList)
	{
		elem.lock()->Add_Y(m_fPopupOffset);
	}

	weak_ptr<CUI_ItemPopup>	pItemPopup = m_pItemPopupQueue.front();

	m_pItemPopupQueue.pop();

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
