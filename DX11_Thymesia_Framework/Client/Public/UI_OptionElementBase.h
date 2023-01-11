#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CUI_OptionElementBase : public CUI_Button
{
public:
	GAMECLASS_H(CUI_OptionElementBase)
	CLONE_H(CUI_OptionElementBase, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg) override;

public:
	FDelegate<_int>				Callback_OnChangeOption;

	virtual void				Call_OnClickLeftArrow();
	virtual void				Call_OnClickRightArrow();

	virtual void				Set_Y(_float fY);

protected:
	virtual void				SetUp_ArrowPos();

protected:
	weak_ptr<CUI_Button>		m_pLeftArrow;
	weak_ptr<CUI_Button>		m_pRightArrow;

protected:
	_int						m_iAmount = 0;

private:
	void						Free();

};

END
