#pragma once
#include "UI.h"


BEGIN(Client)

class CCustomUI final : public CUI
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


protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:

public:
	void Free();
};

END

