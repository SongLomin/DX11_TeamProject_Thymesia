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
		_float m_fCurrentHP_white; //�������ü��
		_float m_fMaxHP_white; //��������ִ�ü��
		_float m_fCurrentHP_Green; // �����ʷϻ�ü��
		_float m_fMaxHP_Green; //�����ʷϻ��ִ�ü��
		_float m_fAtk; //���� ���ݷ�
		_float m_fHitedTime; //���ظ� ���� �ķκ��� ���� �ð�(�����븶���ʱ�ȭ)
		_float m_fRecoveryAlramTime; //���ü��ȸ������ð�
		_float m_fRecoveryTime; //ȸ�������ǽð�
		_float m_fHpBarDisableTime; //ui�𽺿��̺�Ǳ�����ǽð�
		_float m_fCurrentParryingGauge; //�и������������ġ
		_float m_fMaxParryingGauge; //�и��������ִ��ġ


		_uint  m_iCueentParryCount; // �и��ϴ¸����� �и�ī��Ʈ
		_uint  m_iMaxParryCount; //  �и������ִ�ī��Ʈ�Ǹ��и�

		_uint  m_iLifeCount; // 1������ 2������ ���
		
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
