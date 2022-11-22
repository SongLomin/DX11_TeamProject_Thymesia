#pragma once
#include "UI_LerpBar.h"


BEGIN(Client)


class CCustomUI;

class CMonsterParryingBar : public CUI_LerpBar
{
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	weak_ptr< CCustomUI> m_pBorder;

private:
	_float		m_fGauegeRecoveryTime;
	_float		m_fGauegeRecoveryMaxTime;
};

END
