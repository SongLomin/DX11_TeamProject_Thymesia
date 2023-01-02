#pragma once
#include "CustomUI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)


class CEasingComponent_Float;

class CDissolveUI : public CCustomUI
{
	GAMECLASS_H(CDissolveUI);
	CLONE_H(CDissolveUI, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	void			Start_FadeIn(_float fStart, _float fTarget, _float fTime);
	void			Start_FadeOut(_float fStart, _float fTarget, _float fTime);

protected:
	virtual void	SetUp_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;



public:
	void			Call_EndFadeIn();
	void			Call_EndFadeOut();

protected:
	_float				m_fRatio;
	weak_ptr<CTexture>	m_pDissolveTexture;
	weak_ptr< CEasingComponent_Float>	m_pEasingFloat;


	_uint				m_iTextureIndex = 0;

public:
	FDelegate<>			Callback_EndFadeIn;
	FDelegate<>			Callback_EndFadeOut;


public:
	void Free();



};
END

