#pragma once
#include "UI_LerpBar.h"


BEGIN(Client)


class CCustomUI;
class CProgressBar;

class CMonsterParryingBar : public CCustomUI
{
public:
	GAMECLASS_H(CMonsterParryingBar)
		CLONE_H(CMonsterParryingBar, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	weak_ptr< CCustomUI> m_pBorder;
	weak_ptr<CCustomUI>	m_pMainBar;
private:
	_float		m_fGauegeRecoveryTime;
	_float		m_fGauegeRecoveryMaxTime;

	_float		m_fCrurrentParryGauge;
	_float		m_fLerpedParryGauge;

};

END
