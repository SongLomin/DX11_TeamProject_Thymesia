#pragma once
#include "CustomUI.h"


BEGIN(Client)

class CEasingComponent_Float;

class CUI_EvolveMenu_SKillIcon : public CCustomUI
{
public:
	GAMECLASS_H(CUI_EvolveMenu_SKillIcon)
	CLONE_H(CUI_EvolveMenu_SKillIcon, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	

public:
	void			Call_OnEasingFloatConditionEvent(_uint iEventNumber);

public:
	virtual void	SetUp_Component();
	
	_float			Get_Ratio();
	void			StartGauge();
	void			EndGauge();

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:


	_float				m_fRatio = 0.f;
	_float				m_fShakeMag = 5.f;

	weak_ptr<CEasingComponent_Float>	m_pEasingRatio;



private:
	void				Free();
};

END