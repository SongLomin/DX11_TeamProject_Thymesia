#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CEasingComponent_Alpha;
class CEasingComponent_Float;


class CUI_ItemRequirement : public CCustomUI
{
public:
	GAMECLASS_H(CUI_ItemRequirement)
	CLONE_H(CUI_ItemRequirement, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	void		Call_ItemRequireMent(ITEM_NAME eITemName);
	void		Call_RequirementDir();


	void		Call_UseItem(ITEM_NAME eITemName);


public:
	void				Call_EndLerp();
	void				Call_EndFadeIn();
	void				Call_EndTimeOut();




	void				Start_UI();




private:
	virtual void		SetUp_Component() override;


private:
	weak_ptr<CEasingComponent_Alpha> m_pEasingAlpha;
	weak_ptr<CEasingComponent_Float> m_pEasingFloat;

	_bool							m_bIsAnimation = false;

};

END
