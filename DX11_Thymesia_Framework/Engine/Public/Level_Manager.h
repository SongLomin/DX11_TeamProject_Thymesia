#pragma once

#include "Base.h"

/* 현재 게임내에 보여줘야할 레벨을 보관한다. */
/* 레벨교체시에 기존 레벨을 삭제한다. */
/* 기존레벨에서 사용했던 객체와 자원(리소스등)을 정리한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)

public:
	HRESULT Open_Level(_uint iLevelID, shared_ptr<class CLevel> pLevel);
	weak_ptr<CLevel> Get_CurrentLevel() { return m_pCurrentLevel; }
	void Tick(_float fTimeDelta);
	HRESULT Render(ID3D11DeviceContext* pDeviceContext);

	GETSET(_uint, iLastCreatedLevelID)

private:
	shared_ptr<class CLevel>			m_pCurrentLevel;
	_uint					m_iCurrentLevelIDBeforeLevelInitalize = 0;

public:
	virtual void OnDestroy() override;
	void Free();
};

END