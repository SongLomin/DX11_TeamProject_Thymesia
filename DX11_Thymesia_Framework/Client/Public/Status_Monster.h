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
		_float m_fRecoveryAlramTime; //흰색체력회복예고시간
		_float m_fRecoveryTime; //회복까지의시간
		_float m_fHpBarDisableTime; //ui디스에이블되기까지의시간
		_float m_fCurrentParryingGauge; //패링게이지현재수치
		_float m_fMaxParryingGauge; //패링게이지최대수치


		_uint  m_iCueentParryCount; // 패링하는몬스터의 패링카운트
		_uint  m_iMaxParryCount; //  패링몬스터최대카운트되면패링

		_uint  m_iLifeCount; // 1페이지 2페이지 목숨
		
	}MONSTERDESC;
		


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() { return S_OK; }

public:
	virtual _bool   Is_Dead();
	virtual void    Init_Status(const void* pArg);
	virtual void    Add_Damage(const _float& In_fDamage, ATTACK_OPTION eAttackOption);
protected:
	void			Decrease_HP(_float& fCurrentHP, const _float& In_fDamage);
	void			Check_HitedTime();

protected:
	MONSTERDESC     m_tMonsterDesc;
};

END
