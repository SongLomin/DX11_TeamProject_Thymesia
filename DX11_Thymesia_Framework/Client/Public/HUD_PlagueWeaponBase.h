#pragma once
#include "CustomUI.h"
#include "PlayerSkill_System.h"
BEGIN(Client)

class CCustomUI;
class CProgressBar;
class CHUD_Hover;
class CSkill_Base;


class CHUD_PlagueWeaponBase :
    public CCustomUI
{
	GAMECLASS_H(CHUD_PlagueWeaponBase);
	CLONE_H(CHUD_PlagueWeaponBase, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	/*Delegate  From SkillSystem*/
	virtual void		Call_OnChangeSkill(weak_ptr<CSkill_Base> pSkillBase);

	/*Delegate From SkillBase*/
	virtual void		Call_UseStartSkill();
	virtual void		Call_UpdateCoolDown(_float fCoolDownRatio);
	virtual void		Call_EndCoolDown();

public:
	//HoverEvent
	virtual void			Call_HoverFadeEnd(FADER_TYPE eFaderType);
	
protected:
	virtual void			Bind_Player();//플레이어 스킬 시스템을 바인딩하는 함수


protected:
	virtual void			Init_UI();//의자앉을때나, 게임 시작할때 초기화 먹여주기
	void					Init_Icon(SKILL_NAME eSkillName);

	
	


private:
	virtual void			OnEnable(void* pArg) override;

	virtual void			OnDisable() override;

protected:
	CPlayerSkill_System::SOCKET_TYPE m_eSkillSocketType = CPlayerSkill_System::SOCKET_TYPE::SOCKET_END;
	weak_ptr<CCustomUI>		m_pPlagueWeapon_Main;//프레임
	weak_ptr<CProgressBar>	m_pPlagueWeapon_Ready;//쿨타임시에만 나오는 회색 막
	weak_ptr<CProgressBar>	m_pPlagueWeapon_Border;//쿨타임 알려주는 게이지 프레임
	weak_ptr<CCustomUI>		m_pIcon;
	weak_ptr<CHUD_Hover>	m_pHover;//스킬 쿨 다 돌았을시 나오는 이펙트.

protected:
	Engine::FaderDesc m_tFaderDesc; //For HUD_Hover;

public:
	void Free();

};

END