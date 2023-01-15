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
    {
        POTION_DEFAULT, POTION_BUFF, POTION_IMMEDIATE, POTION_END
    };
public:

    typedef struct tagPlayerPotionDesc
    {
        _uint      m_iCurrentPotion;
        _uint      m_iMaxPotion;
        _float      m_fHealingAmount;
        _float      m_fHealingTime;

    }PLAYERPOTIONDESC;

    typedef struct tagPlayerDesc
    {
        _float      m_fCurrentHP;
        _float      m_fMaxHP;
        _float      m_fCurrentMP;
        _float      m_fMaxMP;
        _float      m_fNormalAtk;
        _float      m_fPlagueAtk;
        _float      m_fParryingAtk;
        _float      m_fFeatherAtk;
        _uint      m_iCurrentFeather;
        _uint      m_iMaxFeather;
        _uint      m_iMemory;
        _uint      m_iTalent;

        _uint      m_iLevel;
        _uint      m_iStr;
        _uint      m_iVital;
        _uint      m_iPlague;
        _uint      m_iWound;

        tagPlayerDesc()
        {
            ZeroMemory(this, sizeof(tagPlayerDesc));
		    m_fCurrentHP = 300.f;
            m_fCurrentMP = 150.f;
			m_fMaxHP = 300.f;
			m_fMaxMP = 150.f;
			m_fNormalAtk = 25.f;
			m_fPlagueAtk = 200.f;
			m_fFeatherAtk = 30.f;
			m_fParryingAtk = 50.f;
			m_iLevel = 1;
			m_iStr = 1;
			m_iVital = 1;
			m_iPlague = 1;
			m_iWound = 1;
			m_iMaxFeather = 3;
			m_iMemory = 1000;//시작할때는 알거지로
			m_iTalent = 10;
        }
    }PLAYERDESC;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Start();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) { return S_OK; }


public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;


    virtual void    Write_SaveData(json& Out_Json) override;
    virtual void    Load_SaveData(const json& In_Json) override;

public:
    FDelegate<>         Callback_Update_Status;
    FDelegate<_float>   Callback_ChangeHP;
    FDelegate<_float>   Callback_ChangeMP;
    FDelegate<_uint>    Callback_ChangeFeather;
    FDelegate<_uint>    Callback_RootingMemory;

    FDelegate<_uint, _uint> Callback_ChangePotion;
public:
    virtual _bool   Is_Dead();
    virtual void    Init_Status(const void* pArg);

    void            Add_Str(_uint iStr);
    virtual void    Add_Damage(const _float& In_fDamage);
    void            Add_Memory(_uint    iRootedMemory);
    void            Set_Memory(_uint      iMemory);
    void            Consumed_Mana(_float fRequireMana);
    virtual void   Full_Recovery() override;


    PLAYERDESC Get_PlayerDesc() { return m_tDesc; }
    PLAYERPOTIONDESC Get_CurrentPotionDesc() { return m_PotionDesc[m_iCurrentPotionIndex]; }

    virtual void   Get_Desc(void* Out_pDesc);
    PLAYERDESC      Get_Desc() const;
public://For HPBar
    _float         Get_MaxHP() { return m_tDesc.m_fMaxHP; }
    _float         Get_CurrentHP() { return m_tDesc.m_fCurrentHP; }
   
public://For MPBar
    _float         Get_MaxMP() { return m_tDesc.m_fMaxMP; }
    _float         Get_CurrentMP() { return m_tDesc.m_fCurrentMP; }



    _float         Get_Atk() { return m_tDesc.m_fNormalAtk; }
    virtual void    Set_Desc(void* In_Desc) override;
    void            Heal_Player(const _float fAmount);
    void            ManaHeal_Player(const _float fAmount);


    void            Heal_PlayerFromMaxHP(const _float fRatio);
    void            MPHeal_PlayerFromMaxMP(const _float fRatio);






public: //For Potion
    _bool            Get_UseableCurrentPotion();
    void             Use_Potion();
    _uint            Get_CurrentPotionCount();
    _uint            Get_MaxPotionCount();


private:
    _float         m_fPotionTime;
    _uint         m_iCurrentPotionIndex;
    PLAYERPOTIONDESC   m_PotionDesc[(_uint)POTIONTYPE::POTION_END];

protected:
    PLAYERDESC      m_tDesc; // m_tPlayerDesc
    
private:
    void         Free();

};
END