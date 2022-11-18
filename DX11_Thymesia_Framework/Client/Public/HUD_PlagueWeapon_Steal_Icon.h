#pragma once
#include "DissolveUI.h"


BEGIN(Engine)
CTexture;
END

BEGIN(Client)

class CHUD_PlagueWeapon_Steal_Icon :public CDissolveUI
{
	GAMECLASS_H(CHUD_PlagueWeapon_Steal_Icon);
	CLONE_H(CHUD_PlagueWeapon_Steal_Icon, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT SetUp_ShaderResource() override;

public:
	void Free();


private:
	weak_ptr<CTexture>	m_pMask;
};


END
