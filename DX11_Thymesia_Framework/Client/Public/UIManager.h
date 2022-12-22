#pragma once
#include "Base.h"

BEGIN(Client)

class CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

public:
	void		Set_OpenedMenu(_bool bOpenedMenu);
	_bool		Is_OpenedMenu() { return m_bOpenedMenu; }

private:
	_bool		m_bOpenedMenu = false;

private:
	void		Free();

};


END
