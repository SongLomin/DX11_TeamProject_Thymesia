#pragma once
#include "DissolveUI.h"


BEGIN(Engine)
class CTexture;
class CFader;
END

BEGIN(Client)

class CLoadingTextUI :public CDissolveUI
{
	GAMECLASS_H(CLoadingTextUI);
	CLONE_H(CLoadingTextUI, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	void			Set_DelayAlpha(_float fDelayTime, _float fStartTime);

protected:
	virtual HRESULT SetUp_ShaderResource() override;
public:
	void Free();


private:
	weak_ptr<CTexture>	m_pMask;
	_float				m_fRange = 0.f;
	_float2				m_pt;

private:
	_float					m_fStartDelay = 0.f;
	_float					m_fTick = 0.f;
	_float					m_fTime = 0.f;
	_bool					m_bDelay = false;

};


END
