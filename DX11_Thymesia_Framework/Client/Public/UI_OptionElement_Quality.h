#pragma once
#include "UI_OptionElementBase.h"

BEGIN(Client)

class CCustomUI;

class CUI_OptionElement_Quality : public CUI_OptionElementBase
{
public:
	GAMECLASS_H(CUI_OptionElement_Quality)
		CLONE_H(CUI_OptionElement_Quality, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual void				Call_OnClickLeftArrow();
	virtual void				Call_OnClickRightArrow();

protected:
	virtual void				SetUp_ArrowPos();

private:
	void						ClickArrow();

private:
	weak_ptr<CCustomUI>			m_pOptionState;

	_int						m_iMaxAmount = 2;

private:
	void						Free();

};

END
