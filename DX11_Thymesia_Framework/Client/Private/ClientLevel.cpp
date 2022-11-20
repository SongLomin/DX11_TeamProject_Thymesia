#include "stdafx.h"
#include "ClientLevel.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "GameManager.h"

GAMECLASS_C(CClientLevel)

HRESULT CClientLevel::Initialize()
{
	__super::Initialize();

	GAMEINSTANCE->Set_CreatedLevelIndex(m_eMyLevel);

	return S_OK;
}

void CClientLevel::Load_FromJson(const string& In_szJsonPath, const LEVEL& In_eLevel)
{
	json LoadedJson;

	if (FAILED(CJson_Utility::Load_Json(In_szJsonPath.c_str(), LoadedJson)))
	{
		//DEBUG_ASSERT;
	}
		
	
	for (auto& Elem_GameObjects : LoadedJson["GameObject"])
	{
		weak_ptr<CGameObject> pGameObjectInstance = GAMEINSTANCE->Add_GameObject(Elem_GameObjects["Hash"], (_uint)In_eLevel);

		//객체가 생성되지 않음. Loader에서 프로토타입이 생성됐는지 확인 필요함.
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
		if (!pGameObjectInstance.lock().get())
			cout << "객체가 생성되지 않음. 프로토타입 확인" << (string)Elem_GameObjects["Name"] << endl;
#endif
#endif // _DEBUG
		if (pGameObjectInstance.lock().get())
		{
			pGameObjectInstance.lock()->Set_Enable(Elem_GameObjects["Setting"]["Enable"]);
			pGameObjectInstance.lock()->Load_FromJson(Elem_GameObjects);
		}
	}
}

void CClientLevel::Change_NextLevel(void* pArg)
{
	m_bChangeNextLevel = true;

}

void CClientLevel::Call_FadeOutToLevelChange()
{
	//weak_ptr<CGameObject> temp = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::FADEMASK).front();
	//m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	if (m_bFadeOut)
		return;

	m_bFadeOut = true;

	m_pFadeMask.lock()->CallBack_FadeEnd -= bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	m_pFadeMask.lock()->Set_Enable(false);
	if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_eNextLevel))))
		return;
}

void CClientLevel::Free()
{

}
