#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CEasingComponent_Alpha;

class CUI_EvolveTalent_Active :
    public CCustomUI
{
public:
	GAMECLASS_H(CUI_EvolveTalent_Active)
	CLONE_H(CUI_EvolveTalent_Active, CGameObject)



public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	void		Set_Selected(_bool bSelected);
protected:
	virtual HRESULT SetUp_ShaderResource() override;


private:
	weak_ptr<CTexture>	m_pMaskingTextureCom;

	weak_ptr<CEasingComponent_Alpha>	m_pEasingTransformCom;
	
};

END