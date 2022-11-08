#pragma once
#include "UI.h"


BEGIN(Client)

class CPlayer_ProgressBar final :  public CUI
{
	GAMECLASS_H(CPlayer_ProgressBar);
	CLONE_H(CPlayer_ProgressBar, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void	Set_Ratio(_float _fRatio);


public:
	virtual void OnEventMessage(_uint iArg) override;

public:

protected:
	virtual HRESULT SetUp_ShaderResource() override;


private:
	_float			m_fRatio; //플레이어 체력 비율. 1이 최대값임.

public:
	void Free();

};

END

