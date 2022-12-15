#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CSkill_Base;

class CPlayerSkill_System : public CClientComponent
{
public:
	typedef enum class socketType
	{
		SOCKET_MAIN = 0, SOCKET_SUB, SOCKET_STEAL, SOCKET_END
	}SOCKET_TYPE;

public:
	GAMECLASS_H(CPlayerSkill_System);
	CLONE_H(CPlayerSkill_System, CComponent)


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual void Start() override;

public:
	void					ResetAllSkillCoolDown();

public:
	weak_ptr<CSkill_Base>	Get_MainSkill();


public:
	FDelegate<weak_ptr<CSkill_Base>>	Callback_OnChangeSkill[(_uint)SOCKET_TYPE::SOCKET_END];
	

public:
	void			OnChangeSkill(weak_ptr<CSkill_Base> pSkill, SOCKET_TYPE eType);
private:
	void			Tick_SkillList(_float fTimeDelta);

private:
	weak_ptr<CSkill_Base>		m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_END];
	
public:
	
};


END