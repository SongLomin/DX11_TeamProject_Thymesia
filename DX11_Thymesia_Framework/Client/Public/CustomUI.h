#pragma once
#include "UI.h"


BEGIN(Client)

class CEasingTransform;

class CCustomUI : public CUI
{
public:
	GAMECLASS_H(CCustomUI);
	CLONE_H(CCustomUI, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_PassIndex(_uint _iPassIndex);
	void		Set_DeffuseIndex(_uint _iDeffuseIndex);

protected:
	virtual HRESULT SetUp_ShaderResource() override;



private:
	_uint	m_iDeffuseIndex = 0;
	
public:
	void Free();
};

END

