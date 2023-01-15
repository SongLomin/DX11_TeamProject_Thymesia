#pragma once
#include "UI.h"
#include "Status_Monster.h"	


BEGIN(Client)
class CCustomUI;
class CProgressBar;
class CHUD_Hover;
class CMonster;
class CMonsterParryingBar;
class CStatus_Monster;
class CMonsterHPBar_Base : public CUI
{
public:
    GAMECLASS_H(CMonsterHPBar_Base)
    CLONE_H(CMonsterHPBar_Base, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void	Set_Target(weak_ptr<CBase> pMonster) override;

	void			Reset();
	void			FollowTarget();
	void			Set_Offset(_float3 _vOffset) { m_vOffset = _vOffset; }
	
	
protected:
	virtual void	Bind_EventFunction(weak_ptr<CStatus_Monster> pStatus_Monster);

	//CallEventFuncitons
public:
	void			Call_UpdateHP(CStatus_Monster::MONSTERDESC tDesc);
	void			Call_Update_ParryGauge(_float _fRatio, _bool bLerp);
	void			Call_Damaged_White(_float _fRatio);
	void			Call_Damaged_Green(_float _fRatio);
	void			Call_Damaged_Parry(_float _fRatio);
	
	void			Call_RecoveryAlram();
	void			Call_Recovery();
	void			Call_Disable();
	void			Call_Stun();
	void			Call_Restart();
	virtual void	Call_Full_Recovery();
public:
	virtual void OnEnable(void* pArg) override;
	virtual void OnDisable() override;

protected:
	virtual void			Green_Damaged(_float fRatio);
	virtual void			Set_RecoveryAlram(_bool _bRecovery);
	virtual void			Set_Stun(bool _bStun);

	virtual void			Reset_UI();



protected:
	virtual void    Add_Child(weak_ptr<CUI> pChild);
	virtual void	Check_Track();

protected:
	virtual void	Set_ChildPosFromThis();


private:
	void		Set_DecorationHead();

protected:
    weak_ptr<CCustomUI> m_pBorder; // 0.6f;
	weak_ptr<CCustomUI> m_pDecoration_Head; // 0.0f;

	weak_ptr<CProgressBar>	m_pWhite;
	weak_ptr<CProgressBar>	m_pGreen;


	//특정 상황에서만 켜지는 녀석들
	weak_ptr<CHUD_Hover>	m_pGreenTrack;
	weak_ptr<CCustomUI>		m_pTrack;
	weak_ptr<CHUD_Hover>	m_pRecovery;
	weak_ptr<CHUD_Hover>	m_pStunned;
	
	
	
	weak_ptr<CMonster>		m_pTarget;


	weak_ptr<CMonsterParryingBar> m_pParryingBar;


	_float3					m_vOffset;


public:
	void		Call_GreenTrackFadeEnd(FADER_TYPE eType);
	_bool		m_bStun = false;
	

	
};

END

