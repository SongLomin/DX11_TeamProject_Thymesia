#pragma once

#include "Status_Monster.h"


BEGIN(Client)

class CStatus_Boss : public CStatus_Monster
{
public:
	GAMECLASS_H(CStatus_Boss)
	CLONE_H(CStatus_Boss, CComponent)

public:
	virtual HRESULT Initialize(void* pArg);

	virtual void			Update_HitedTime(_float fTimeDelta);


private:
	void			Free() { return; }
};

END
