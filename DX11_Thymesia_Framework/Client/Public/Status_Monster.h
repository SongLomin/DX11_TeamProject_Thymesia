#pragma once

#include "Status.h"

BEGIN(Client)

class CStatus_Monster : public CStatus
{
public:
	GAMECLASS_H(CStatus_Monster)
		CLONE_H(CStatus_Monster, CComponent)
public:
	typedef struct tagMonsterDesc
	{
		_float m_fCurrentHP_white; //몬스터흰색체력
		_float m_fMaxHP_white; //몬스터흰색최대체력
		_float m_fCurrentHP_Green; // 몬스터초록색체력
		_float m_fMaxHP_Green; //몬스터초록색최대체력
		_float m_fAtk; //몬스터 공격력
		_float m_fHitedTime; //피해를 받은 후로부터 지난 시간(맞을대마다초기화)
		_float m_fParryGaugeRecoveryTime; //피해를 받은 후로부터 지난 시간(맞을대마다초기화)

		_float m_fRecoveryAlramTime; //흰색체력회복예고시간
		_float m_fRecoveryTime; //회복까지의시간
		_float m_fRecoveryAmountPercentageFromSecond;//초당 몇퍼센트까지 회복할껀지(0~1로)
		_float m_fRecoveryMag;//배율(만약에, 깃털 맞으면 회복 반깎)
		_float m_fHpBarDisableTime; //ui디스에이블되기까지의시간
		_float m_fCurrentParryingGauge; //패링게이지현재수치
		_float m_fMaxParryingGauge; //패링게이지최대수치
		_uint	m_iDropMemory;//떨구는 돈

		_uint  m_iCueentParryCount; // 패링하는몬스터의 패링카운트
		_uint  m_iMaxParryCount; //  패링몬스터최대카운트되면패링

		_uint  m_iLifeCount; // 1페이지 2페이지 목숨

		string	m_szModelKey;
		
	}MONSTERDESC;
	
	//Delegate
public:
	FDelegate<MONSTERDESC>		Callback_UpdateHP;
	FDelegate<_float>			CallBack_Damged_White;
	FDelegate<_float>			CallBack_Damged_Green;
	FDelegate<>					CallBack_UI_Disable;
	FDelegate<>					CallBack_RecoeoryStart;
	FDelegate<>					CallBack_RecoeoryAlram;
	FDelegate<>					CallBack_ReStart;
	FDelegate<_float, _bool>	CallBack_UpdateParryGauge;
	FDelegate<>					Callback_Full_Recovery;
	
	FDelegate<>					Callback_NextPhase;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) { return S_OK; }


public:
	void			Restart();

public:
	virtual _bool   Is_Dead();
	virtual void    Init_Status(const void* pArg);
	virtual void    Add_Damage(const _float In_fDamage, ATTACK_OPTION eAttackOption) override;
	virtual void    Full_Recovery() override;
	virtual _float	Get_WhiteRatio() {
		return m_tMonsterDesc.m_fCurrentHP_white / m_tMonsterDesc.m_fMaxHP_white
			;
	}
	
	virtual _float	Get_GreenRatio() {
		return m_tMonsterDesc.m_fCurrentHP_Green/ m_tMonsterDesc.m_fMaxHP_Green
			;
	}
	const tagMonsterDesc& Get_Desc() { return m_tMonsterDesc; }
	_float			Get_Atk() { return m_tMonsterDesc.m_fAtk; }
	virtual void    Get_Desc(void* Out_pDesc);

	void			Add_ParryGauge(const _float In_fDamage);
	_bool			Is_Groggy() const;

	void			Heal(_float fHealAmount);

protected:
	void			Decrease_White_HP(const _float In_fDamage);
	void			Decrease_Green_HP(const _float In_fDamage);

	void			Set_ParryRecoveryTime(const _float fRatio);

	virtual void	Update_HitedTime(_float fTimeDelta);
	void			Update_ParryRecoveryTime(_float fTimeDelta);

	void			Init_StatusFromMonsterType(MONSTERTYPE eMonsterType);


protected:
	MONSTERTYPE		m_eMonsterType;
	MONSTERDESC     m_tMonsterDesc;




private:
	void			Free();
};

END
