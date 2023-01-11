#pragma once
#include "UI_OptionElementBase.h"

BEGIN(Client)

class CCustomUI;

class CUI_OptionElement_OnOff : public CUI_OptionElementBase
{
public:
	GAMECLASS_H(CUI_OptionElement_OnOff)
	CLONE_H(CUI_OptionElement_OnOff, CGameObject)

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

private:
	void						Free();

};

END
