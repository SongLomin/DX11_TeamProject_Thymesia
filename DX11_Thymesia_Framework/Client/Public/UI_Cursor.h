#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CUI_Cursor : public CCustomUI
{
public:
	GAMECLASS_H(CUI_Cursor)
	CLONE_H(CUI_Cursor, CGameObject)
public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	
private:
	void		CreateCursor();

private:
	void		Free();


private:
	_float		m_fCursorSize = 32.f;
	POINT		m_tMousePt;
};

END
