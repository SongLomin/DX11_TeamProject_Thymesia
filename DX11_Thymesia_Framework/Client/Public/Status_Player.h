#pragma once
#include "Status.h"

BEGIN(Client)


class CStatus_Player : public CStatus
{
public:
	GAMECLASS_H(CStatus_Player)
	CLONE_H(CStatus_Player, CComponent)

public:
	enum class POTIONTYPE
	{//���� �Ϲ� : �� �ٷ���(���µ� �ð��� �ɸ�)
		//���� ���� : �� ������ �ʰ���(��� ���� ���� ����)�ȴ� �ð� ��.
		//���� ��� : �� ������. �ٵ� ���¼ӵ� ��������
		POTION_DEFAULT, POTION_BUFF, POTION_IMMEDIATE, POTION_END
	};
public:

	typedef struct tagPlayerPotionDesc
	{
		_uint		m_iCurrentPotion;
		_uint		m_iMaxPotion;
		_float		m_fHealingAmount;//�ִ�ü���� ��ŭ�� ȸ�������ִ°�.
		_float		m_fHealingTime; //���ʸ�ŭ ȸ����ų�ǰ�. 0�ʸ� �ٷ� ���� AMount��ŭ �ٷ� ȸ������.
		/*
			HP += ((MaxHP * (m_fHealingAmount / m_fHealingTime))) * fTimeDelta;
			�׸��� ���� ȸ���� m_fHealingTime��ŭ �����ָ� ��.
		*/
	}PLAYERPOTIONDESC;

	typedef struct tagPlayerDesc
	{
		_float		m_fCurrentHP;//����ü��
		_float		m_fMaxHP;//�ִ�ü��
		_float		m_fCurrentMP;//������
		_float		m_fMaxMP;//�ִ���
		_float		m_fNormalAtk;//�Ϲݰ��ݵ�����
		_float		m_fPlagueAtk;//Ŭ�� ���� ������
		_float		m_fParryingAtk;//�и�������
		_float		m_fFeatherAtk;
		_uint		m_iCurrentFeather;
		_uint		m_iMaxFeather;
		_uint		m_iMemory;


		_uint		m_iLevel;
		_uint		m_iStr;
		_uint		m_iVital;
		_uint		m_iPlague;
		_uint		m_iWound;


	}PLAYERDESC;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() { return S_OK; }


public:
	FDelegate<_float>	Callback_ChangeHP;
	FDelegate<_float>	Callback_ChangeMP;
	FDelegate<_uint>	Callback_ChangePotion;
	FDelegate<_uint>	Callback_ChangeFeather;

public:
	virtual _bool   Is_Dead();
	virtual void    Init_Status(const void* pArg);
	virtual void    Add_Damage(const _float& In_fDamage);

	virtual void	Full_Recovery();//��Ȱ�̳�, ���� ������ �ߵ��ϴ� ���ȸ��

	PLAYERDESC Get_PlayerDesc() {return m_tDesc;}
	PLAYERPOTIONDESC Get_CurrentPotionDesc() { return m_PotionDesc[m_iCurrentPotionIndex]; }

	virtual void	Get_Desc(void* Out_pDesc);
	PLAYERDESC		Get_Desc() const;

	_float			Get_MaxHP() { return m_tDesc.m_fMaxHP; }
	_float			Get_CurrentHP() { return m_tDesc.m_fCurrentHP; }

	void Heal_Player(const _float fAmount);

	_float		Get_Atk() { return m_tDesc.m_fNormalAtk; }

private:
	_float			m_fPotionTime;
	_uint			m_iCurrentPotionIndex;
	PLAYERPOTIONDESC	m_PotionDesc[(_uint)POTIONTYPE::POTION_END];


	//������ �÷��̾� ���°� �� ��ӹ��� ��Ȳ�� ����������, Ȥ�ø��� ���ܳ���
protected:
	PLAYERDESC		m_tDesc; // m_tPlayerDesc

private:
	void			Free();

};
END
