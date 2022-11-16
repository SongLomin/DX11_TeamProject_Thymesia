#pragma once
#include "Player_ProgressBar.h"


BEGIN(Engine)

class CTexture;

END

BEGIN(Client)

class CHUD_PlagueWeapon_Steal_Icon : public CPlayer_ProgressBar
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

private:
	weak_ptr<CTexture>	m_pDissolveTexture;

	weak_ptr<CTexture>	m_pDissolveMaskTexture;

	_uint				m_iTextureIndex = 0;

public:
	void Free();



};
END

