#pragma once
#include "UI_LerpBar.h"


BEGIN(Client)


class CCustomUI;
class CProgressBar;


class CMonsterParryingBar : public CUI_LerpBar
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
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

	virtual void		Reset();
	virtual void		Set_UIPosition(const _float fX, const _float fY) override;


	void				Set_Ratio(_float fRatio, _bool bRatio);

private:
	weak_ptr< CCustomUI> m_pBorder;
	weak_ptr<CProgressBar>	m_pMainBar;

private:
	_float		m_fGauegeRecoveryTime;
	_float		m_fCrurrentParryGauge;
	_float		m_fLerpedParryGauge;
	_float		m_fAmount;
};

END
