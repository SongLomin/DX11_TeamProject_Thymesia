#pragma once
#include "UI.h"


BEGIN(Client)

class CEasingTransform;
class CEasingComponent_Alpha;

class CCustomUI : public CUI
{
	friend class CPreset_UIDesc;
public:
	GAMECLASS_H(CCustomUI);
	CLONE_H(CCustomUI, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void	SetUp_Component();

public:
	void		Set_PassIndex(_uint _iPassIndex);
	void		Set_DeffuseIndex(_uint _iDeffuseIndex);
	void		Set_RenderGroup(RENDERGROUP eRenderGroup) { m_eRenderGroup = eRenderGroup; }

protected:
	virtual HRESULT SetUp_ShaderResource() override;



protected:
	_uint	m_iDeffuseIndex = 0;
public:
	void Free();
};

END

