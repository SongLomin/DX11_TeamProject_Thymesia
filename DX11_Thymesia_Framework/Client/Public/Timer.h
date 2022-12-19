#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CTimer : public CClientComponent
{
public:
	GAMECLASS_H(CTimer);
	CLONE_H(CTimer, CComponent)
	SHALLOW_COPY(CTimer)

public:
	// CClientComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

};

END
