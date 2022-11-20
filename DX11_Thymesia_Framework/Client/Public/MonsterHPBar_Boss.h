#pragma once
#include "MonsterHPBar_Base.h"

class CMonsterHPBar_Boss : public CMonsterHPBar_Base
{
public:
	GAMECLASS_H(CMonsterHPBar_Boss)
	CLONE_H(CMonsterHPBar_Boss, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	Set_Stun(bool _bStun);

protected:
	virtual void	Set_ChildPosFromThis();

private:
	
};

