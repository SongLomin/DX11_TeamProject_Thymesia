#pragma once
#include "Base.h"

BEGIN(Client)

class CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

public:
	void		Set_OpenedMenu(_bool bOpenedMenu);
	_bool		Is_OpenedMenu() { return m_bOpenedMenu; }


	void		Set_UIAnimation(_bool bUIAnimation) { m_bIsAnimation = bUIAnimation; }
	_bool		Is_Animation() { return m_bIsAnimation; }


private:
	_bool		m_bOpenedMenu = false;
	_bool		m_bIsAnimation = false;
private:
	void		Free();

};


END
