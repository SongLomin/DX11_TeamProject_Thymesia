#pragma once
#include "CustomUI.h"


BEGIN(Engine)
class CTexture;
class CEasingComponent_Alpha;
END

BEGIN(Client)


class CUI_EvolveMenu_SelectDecoration : public CCustomUI
{
public:
	GAMECLASS_H(CUI_EvolveMenu_SelectDecoration)
	CLONE_H(CUI_EvolveMenu_SelectDecoration, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	void		Start_Animation();

protected:
	virtual HRESULT SetUp_ShaderResource() override;


private:
	weak_ptr<CTexture>	m_pMaskingTextureCom;

	weak_ptr<CEasingComponent_Alpha>	m_pEasingAlphaCom;
	_float2						m_vUXOffset;
};

END

