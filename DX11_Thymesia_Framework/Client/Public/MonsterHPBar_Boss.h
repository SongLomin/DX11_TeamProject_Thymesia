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
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void			Set_Target(weak_ptr<CBase> pTarget) override;


protected:
	virtual void			Bind_EventFunction(weak_ptr<CStatus_Monster> pStatus_Monster) override;
	virtual void			Create_Decoration(weak_ptr<CStatus_Monster> pStatus_Monster);
	virtual void			Call_Full_Recovery() override;

public:
	virtual void	Set_Stun(bool _bStun);

protected:
	virtual void	Set_ChildPosFromThis();


private:
	list<weak_ptr<CCustomUI>>		m_listLifeDecoration;



private:
	virtual void			OnEnable(void* pArg);
	virtual void			OnDisable() override;


public:
	void			Call_NextPhase();

};

END
