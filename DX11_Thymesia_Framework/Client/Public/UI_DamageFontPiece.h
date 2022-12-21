#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CUI_DamageFontPiece : public CCustomUI
{
public:
	GAMECLASS_H(CUI_DamageFontPiece)
	CLONE_H(CUI_DamageFontPiece, CGameObject)

private:
	void	Free() {};


};

END
