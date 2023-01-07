#pragma once
#include "CustomUI.h"

BEGIN(Engine)

class CTexture;

END


BEGIN(Client)

class CEasingComponent_Alpha;

class CUI_BloodOverlay : public CCustomUI
{
public:
	GAMECLASS_H(CUI_BloodOverlay)
	CLONE_H(CUI_BloodOverlay, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void			Call_Overlay(_float fLifeTime);

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	virtual void	SetUp_Component() override;

	virtual void	Call_LerpEnd();

	//Variable
private:
	weak_ptr<CEasingComponent_Alpha>		m_pEasingAlpha;
	weak_ptr<CTexture>						m_pMaskTexture;

private:
	void			Free() {};

};

END
