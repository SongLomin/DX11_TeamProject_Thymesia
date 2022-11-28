#pragma once
#include "UI_Container.h"
#include <queue>

BEGIN(Client)

class CCustomUI;

class CUI_Script :
    public CUI_Container
{
public:
	GAMECLASS_H(CUI_Script)
		CLONE_H(CUI_Script, CGameObject)

public:
	// CUI_Container을(를) 통해 상속됨
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;

private:
	queue<weak_ptr<CCustomUI>>	m_ScriptQueue;


};

END
