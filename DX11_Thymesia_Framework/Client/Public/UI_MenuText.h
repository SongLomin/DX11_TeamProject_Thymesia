#pragma once
#include "UI.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCustomUI;

class CUI_MenuText
{
public:
	CUI_MenuText();
	
private:

	TEXTINFO	m_MainTextInfo;
	TEXTINFO	m_SubTextInfo;
	_float2 m_pt;
};
END

