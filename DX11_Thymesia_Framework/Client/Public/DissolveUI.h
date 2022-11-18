#pragma once
#include "CustomUI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

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
	virtual HRESULT Render();

protected:
	virtual HRESULT SetUp_ShaderResource() override;

protected:
	_float				m_fRatio;

	weak_ptr<CTexture>	m_pDissolveTexture;
	
	_uint				m_iTextureIndex = 0;



public:
	void Free();



};
END

