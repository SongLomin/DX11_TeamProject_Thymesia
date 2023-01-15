#include "stdafx.h"
#include "Status_Player.h"
#include "Status.h"
#include "GameInstance.h"
#include "Engine_Defines.h"
#include "GameManager.h"
GAMECLASS_C(CStatus_Player)
CLONE_C(CStatus_Player, CComponent)


HRESULT CStatus_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStatus_Player::Initialize(void* pArg)
{
    Init_Status(pArg);

    m_szFieldName = "Status_Player";

    return S_OK;
}

void CStatus_Player::Start()
{
    __super::Start();
    Init_Status(nullptr);
}

void CStatus_Player::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_fPotionTime -= fTimeDelta;


    if (KEY_INPUT(KEY::Z, KEY_STATE::TAP))
    {
        m_tDesc.m_iMemory += 100000;
        Callback_Update_Status();
    }

  
    //if (m_fPotionTime >= 0.f)
    //{
    //    m_tDesc.m_fCurrentHP += ((m_tDesc.m_fMaxHP * m_PotionDesc[m_iCurrentPotionIndex].m_fHealingAmount) / m_PotionDesc[m_iCurrentPotionIndex].m_fHealingTime) * fTimeDelta;
    //}
}

void CStatus_Player::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

void CStatus_Player::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

  
}

void CStatus_Player::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

  
}

void CStatus_Player::Write_SaveData(json& Out_Json)
{
    Out_Json[m_szFieldName]["m_fMaxHP"] = m_tDesc.m_fMaxHP;
    Out_Json[m_szFieldName]["m_fMaxMP"] = m_tDesc.m_fMaxMP;
    Out_Json[m_szFieldName]["m_fNormalAtk"] = m_tDesc.m_fNormalAtk;
    Out_Json[m_szFieldName]["m_fPlagueAtk"] = m_tDesc.m_fPlagueAtk;
    Out_Json[m_szFieldName]["m_fParryingAtk"] = m_tDesc.m_fParryingAtk;
    Out_Json[m_szFieldName]["m_iLevel"] = m_tDesc.m_iLevel;
    Out_Json[m_szFieldName]["m_iStr"] = m_tDesc.m_iStr;
    Out_Json[m_szFieldName]["m_iVital"] = m_tDesc.m_iVital;
    Out_Json[m_szFieldName]["m_iPlague"] = m_tDesc.m_iPlague;
    Out_Json[m_szFieldName]["m_iMemory"] = m_tDesc.m_iMemory;
    Out_Json[m_szFieldName]["m_iWound"] = m_tDesc.m_iWound;

}

void CStatus_Player::Load_SaveData(const json& In_Json)
{
    json LoadedJson;
    string                  m_szClientSavePath = "../Bin/ClientComponentData/Corvus/SaveData.json";

    if (SUCCEEDED(CJson_Utility::Load_Json(m_szClientSavePath.c_str(), LoadedJson)))
    {
        (LoadedJson);
        //DEBUG_ASSERT;
    }
    if (LoadedJson.find("Status_Player") == LoadedJson.end())
    {
        Callback_Update_Status();
        return;
    }
    json Status_PlayerJson = LoadedJson["Status_Player"];

    m_tDesc.m_fMaxHP = Status_PlayerJson["m_fMaxHP"];
    m_tDesc.m_fMaxMP = Status_PlayerJson["m_fMaxMP"];
    m_tDesc.m_fNormalAtk = Status_PlayerJson["m_fNormalAtk"];
    m_tDesc.m_fPlagueAtk = Status_PlayerJson["m_fPlagueAtk"];
    m_tDesc.m_fParryingAtk = Status_PlayerJson["m_fParryingAtk"];
    m_tDesc.m_iLevel = Status_PlayerJson["m_iLevel"];
    m_tDesc.m_iStr = Status_PlayerJson["m_iStr"];
    m_tDesc.m_iVital = Status_PlayerJson["m_iVital"];
    m_tDesc.m_iPlague = Status_PlayerJson["m_iPlague"];
    m_tDesc.m_iMemory = Status_PlayerJson["m_iMemory"];
    m_tDesc.m_iWound = Status_PlayerJson["m_iWound"];

    m_tDesc.m_fCurrentHP = m_tDesc.m_fMaxHP;
    m_tDesc.m_fCurrentMP = m_tDesc.m_fMaxMP;

    Callback_Update_Status();
}

_bool CStatus_Player::Is_Dead()
{
    return m_tDesc.m_fCurrentHP < 1.f ? true : false;
}

void CStatus_Player::Init_Status(const void* pArg)
{
    ZeroMemory(&m_tDesc, sizeof(PLAYERDESC));
    m_tDesc.m_fCurrentHP = 300.f;
    m_tDesc.m_fCurrentMP = 150.f;
    m_tDesc.m_fMaxHP = 300.f;
    m_tDesc.m_fMaxMP = 150.f;
    m_tDesc.m_fNormalAtk = 25.f;
    m_tDesc.m_fPlagueAtk = 200.f;
    m_tDesc.m_fFeatherAtk = 30.f;
    m_tDesc.m_fParryingAtk = 50.f;
    m_tDesc.m_iLevel = 1;
    m_tDesc.m_iStr = 1;
    m_tDesc.m_iVital = 1;
    m_tDesc.m_iPlague = 1;
    m_tDesc.m_iWound = 1;
    m_tDesc.m_iMaxFeather = 3;
    m_tDesc.m_iMemory = 1000;//시작할때는 알거지로
    m_tDesc.m_iTalent = 10;

    m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_iMaxPotion = 3;
    m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_fHealingAmount = 150.f;
    m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_fHealingTime = 0.f;//0초면 바로 회복됨.

    m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_iMaxPotion = 0;
    m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_fHealingAmount = 200.f;
    m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_fHealingTime = 5.f;//0초면 바로 회복됨.

    m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_iMaxPotion = 0;
    m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_fHealingAmount = 75.f;
    m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_fHealingTime = 0.1f;//0초면 바로 회복됨.

    m_iCurrentPotionIndex = 0;
    Full_Recovery();
}

void CStatus_Player::Add_Str(_uint iStr)
{
    m_tDesc.m_iStr += iStr;
    m_tDesc.m_fNormalAtk = 25 + (((_float)m_tDesc.m_iStr - 1) * 3.f);
}

void CStatus_Player::Add_Damage(const _float& In_fDamage)
{
    if (0.f > In_fDamage)
        assert(0);

    Decrease_HP(m_tDesc.m_fCurrentHP, In_fDamage);
    Callback_ChangeHP(m_tDesc.m_fCurrentHP);
}

void CStatus_Player::Add_Memory(_uint iRootedMemory)
{
    m_tDesc.m_iMemory += iRootedMemory;

    Callback_RootingMemory(m_tDesc.m_iMemory);
}

void CStatus_Player::Set_Memory(_uint iMemory)
{
    m_tDesc.m_iMemory = iMemory;

    Callback_RootingMemory(m_tDesc.m_iMemory);
}

void CStatus_Player::Consumed_Mana(_float fRequireMana)
{
    if (fRequireMana > m_tDesc.m_fMaxMP) 
    {
        DEBUG_ASSERT;
    }
    
    if (m_tDesc.m_fCurrentMP < fRequireMana)
    {
        m_tDesc.m_fCurrentMP = 0.f;
    }
    else
    {
        m_tDesc.m_fCurrentMP -= fRequireMana;
    }
    Callback_ChangeMP(m_tDesc.m_fCurrentMP);
}

void CStatus_Player::Full_Recovery()
{
    Heal_Player(m_tDesc.m_fMaxHP);

    m_tDesc.m_fCurrentMP = m_tDesc.m_fMaxMP;
    
    Callback_ChangeMP(m_tDesc.m_fCurrentMP);
   
    m_tDesc.m_iCurrentFeather = m_tDesc.m_iMaxFeather;

    m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_iCurrentPotion =
        m_PotionDesc[(_uint)POTIONTYPE::POTION_DEFAULT].m_iMaxPotion;

    m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_iCurrentPotion =
        m_PotionDesc[(_uint)POTIONTYPE::POTION_BUFF].m_iMaxPotion;

    m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_iCurrentPotion =
        m_PotionDesc[(_uint)POTIONTYPE::POTION_IMMEDIATE].m_iMaxPotion;

    Callback_ChangePotion(m_PotionDesc[m_iCurrentPotionIndex].m_iCurrentPotion, m_PotionDesc[m_iCurrentPotionIndex].m_iMaxPotion);
}

void CStatus_Player::Get_Desc(void* Out_pDesc)
{
    memcpy(Out_pDesc, &m_tDesc, sizeof(PLAYERDESC));
}

CStatus_Player::PLAYERDESC CStatus_Player::Get_Desc() const
{
    return m_tDesc;
}

_bool CStatus_Player::Get_UseableCurrentPotion()
{
    return m_PotionDesc[m_iCurrentPotionIndex].m_iCurrentPotion > 0 ? true : false;
}

void CStatus_Player::Use_Potion()
{
    m_PotionDesc[m_iCurrentPotionIndex].m_iCurrentPotion -= 1;

    if ((m_tDesc.m_fMaxHP - m_tDesc.m_fCurrentHP) < 0.1f)
    {
        Callback_ChangePotion(m_PotionDesc[m_iCurrentPotionIndex].m_iCurrentPotion, m_PotionDesc[m_iCurrentPotionIndex].m_iMaxPotion);
        return;
    }

    m_tDesc.m_fCurrentHP += m_PotionDesc[m_iCurrentPotionIndex].m_fHealingAmount;
    
    if (m_tDesc.m_fCurrentHP >= m_tDesc.m_fMaxHP)
        m_tDesc.m_fCurrentHP = m_tDesc.m_fMaxHP;
    
    Callback_ChangeHP(m_tDesc.m_fCurrentHP);
    Callback_ChangePotion(m_PotionDesc[m_iCurrentPotionIndex].m_iCurrentPotion, m_PotionDesc[m_iCurrentPotionIndex].m_iMaxPotion);
}

void CStatus_Player::Heal_Player(const _float fAmount)
{
    m_tDesc.m_fCurrentHP += fAmount;
    if (m_tDesc.m_fCurrentHP >= m_tDesc.m_fMaxHP)
        m_tDesc.m_fCurrentHP = m_tDesc.m_fMaxHP;

    Callback_ChangeHP(m_tDesc.m_fCurrentHP);
}

void CStatus_Player::ManaHeal_Player(const _float fAmount)
{
    m_tDesc.m_fCurrentMP += fAmount;
    if (m_tDesc.m_fCurrentMP >= m_tDesc.m_fMaxMP)
        m_tDesc.m_fCurrentMP = m_tDesc.m_fMaxMP;

    Callback_ChangeMP(m_tDesc.m_fCurrentMP);
}

void CStatus_Player::Heal_PlayerFromMaxHP(const _float fRatio)
{
    _float fHealAmount = m_tDesc.m_fMaxHP * fRatio;
        
    Heal_Player(fHealAmount);
}

void CStatus_Player::MPHeal_PlayerFromMaxMP(const _float fRatio)
{
    _float fHealAmount = m_tDesc.m_fMaxMP * fRatio;

    ManaHeal_Player(fHealAmount);
}

_uint CStatus_Player::Get_CurrentPotionCount()
{
    return m_PotionDesc[m_iCurrentPotionIndex].m_iCurrentPotion;
}

_uint CStatus_Player::Get_MaxPotionCount()
{
    return m_PotionDesc[m_iCurrentPotionIndex].m_iMaxPotion;
}

void CStatus_Player::Set_Desc(void* In_Desc)
{
    PLAYERDESC* Temp = (PLAYERDESC*)(In_Desc);
    memcpy(&m_tDesc, Temp, sizeof(PLAYERDESC));
    Callback_Update_Status();
}

void CStatus_Player::Free()
{

}