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


    }PLAYERDESC;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Start();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render() { return S_OK; }


public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

public:
    FDelegate<>         Callback_Update_Status;
    FDelegate<_float>   Callback_ChangeHP;
    FDelegate<_float>   Callback_ChangeMP;
    FDelegate<_uint>    Callback_ChangeFeather;


    FDelegate<_uint, _uint> Callback_ChangePotion;
public:
    virtual _bool   Is_Dead();
    virtual void    Init_Status(const void* pArg);
    virtual void    Add_Damage(const _float& In_fDamage);

    virtual void   Full_Recovery();

    PLAYERDESC Get_PlayerDesc() { return m_tDesc; }
    PLAYERPOTIONDESC Get_CurrentPotionDesc() { return m_PotionDesc[m_iCurrentPotionIndex]; }

    virtual void   Get_Desc(void* Out_pDesc);
    PLAYERDESC      Get_Desc() const;

public://For HPBar
    _float         Get_MaxHP() { return m_tDesc.m_fMaxHP; }
    _float         Get_CurrentHP() { return m_tDesc.m_fCurrentHP; }
   


    _float         Get_Atk() { return m_tDesc.m_fNormalAtk; }
    virtual void    Set_Desc(void* In_Desc) override;
    void            Heal_Player(const _float fAmount);



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