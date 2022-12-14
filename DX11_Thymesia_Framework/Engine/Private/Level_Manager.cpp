#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager);

HRESULT CLevel_Manager::Open_Level(_uint iLevelID, shared_ptr<CLevel> pLevel)
{
	/* 이전레벨에서 사용한 자원들을 모두 소거한다. */
	if(nullptr != m_pCurrentLevel)
	{


		if (GET_SINGLE(CGameInstance)->Clear(m_iCurrentLevelIDBeforeLevelInitalize))
			return E_FAIL;
	}

	m_pCurrentLevel = pLevel;

	m_iCurrentLevelIDBeforeLevelInitalize = iLevelID;

	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render(pDeviceContext);
}

void CLevel_Manager::OnDestroy()
{
	m_pCurrentLevel.reset();
}

void CLevel_Manager::Free()
{
	//Safe_Release(m_pCurrentLevel);
}
