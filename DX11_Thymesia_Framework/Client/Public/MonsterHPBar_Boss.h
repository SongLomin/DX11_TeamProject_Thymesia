#pragma once
#include "MonsterHPBar_Base.h"

BEGIN(Client)

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
	virtual void			Set_Owner(weak_ptr<CMonster> pMonster) override;

public:
	virtual void	Set_Stun(bool _bStun);
	void			Call_NextPhase();
protected:
	virtual void	Set_ChildPosFromThis();


private:
	virtual void			OnEnable(void* pArg);
	virtual void			OnDisable() override;


private:
	list<weak_ptr<CCustomUI>>		m_listLifeDecoration;
};

END
