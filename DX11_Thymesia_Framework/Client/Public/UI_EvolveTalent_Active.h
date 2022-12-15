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
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	void		Set_Click(_bool bSelected);
	void		Animation_MouseOver();
	void		Animation_MouseOut();


protected:
	virtual HRESULT SetUp_ShaderResource() override;


private:
	weak_ptr<CTexture>	m_pMaskingTextureCom;

	weak_ptr<CEasingComponent_Alpha>	m_pEasingTransformCom;
	_uint								m_iDissolveIndex;
	_bool								m_bSelected;
public:
	void								Call_SelectedTrueEnd();

	void								Call_SelectedFalseEnd();
};

END