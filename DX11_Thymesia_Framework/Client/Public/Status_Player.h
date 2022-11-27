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
	{//포션 일반 : 피 바로참(쓰는데 시간좀 걸림)
		//포션 버프 : 피 서서히 늦게참(대신 차는 양이 많음)꽂는 시간 김.
		//포션 긴급 : 피 조금참. 근데 쓰는속도 ㅈㄴ빠름
		POTION_DEFAULT, POTION_BUFF, POTION_IMMEDIATE, POTION_END
	};
public:

	typedef struct tagPlayerPotionDesc
	{
		_uint		m_iCurrentPotion;
		_uint		m_iMaxPotion;
		_float		m_fHealingAmount;//최대체력의 얼만큼을 회복시켜주는가.
		_float		m_fHealingTime; //몇초만큼 회복시킬건가. 0초면 바로 위에 AMount만큼 바로 회복박힘.
		/*
			HP += ((MaxHP * (m_fHealingAmount / m_fHealingTime))) * fTimeDelta;
			그리고 이제 회복을 m_fHealingTime만큼 시켜주면 됨.
		*/
	}PLAYERPOTIONDESC;

	typedef struct tagPlayerDesc
	{
		_float		m_fCurrentHP;//현재체력
		_float		m_fMaxHP;//최대체력
		_float		m_fCurrentMP;//현재기력
		_float		m_fMaxMP;//최대기력
		_float		m_fNormalAtk;//일반공격데미지
		_float		m_fPlagueAtk;//클로 공격 데미지
		_float		m_fParryingAtk;//패링데미지
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

	virtual void	Full_Recovery();//부활이나, 의자 앉을시 발동하는 모든회복

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


	//어차피 플레이어 상태가 뭐 상속받을 상황은 없을테지만, 혹시몰라서 남겨놓음
protected:
	PLAYERDESC		m_tDesc; // m_tPlayerDesc

private:
	void			Free();

};
END
