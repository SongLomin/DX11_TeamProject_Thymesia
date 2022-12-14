#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CProgressBar : public CCustomUI
{
	GAMECLASS_H(CProgressBar);
	CLONE_H(CProgressBar, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	void	Set_Ratio(_float _fRatio);
	void	Set_PassIndex(_uint _iPassIndex);
	void	Decrease_Ratio(_float fRatio);
	void	Increase_Ratio(_float fRatio);


public:
	_float	Get_Ratio() { return m_fRatio; }

public:
	virtual void OnEventMessage(_uint iArg) override;

public:

protected:
	virtual HRESULT SetUp_ShaderResource() override;


protected:
	_float			m_fRatio; //플레이어 체력 비율. 1이 최대값임.

public:
	void Free();

};

END

