#pragma once
#include "Character.h"

BEGIN(Engine)

class CPhysXCollider;
class CPhysXController;
END

BEGIN(Client)

class CMonster;
class CPlayerStateBase;
class CCorvus_DefaultSaber;
class CCorvus_DefaultDagger;
class CWeapon;
class CTalent_Effect;
class CInventory;


class CPlayer :
    public CCharacter
{
    GAMECLASS_H(CPlayer);
    CLONE_H(CPlayer, CGameObject);

public:
    void Set_TargetMonster(weak_ptr<CMonster> In_pMonster);
    weak_ptr<CMonster> Get_TargetMonster() const;


protected:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;

    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext) override;


public: /* For. Talent */
    void Bind_TalentEffects(weak_ptr<CTalent_Effect> pTalentEffect);
    void UnBind_TalentEffects(weak_ptr<CTalent_Effect> pTalentEffect);
    _flag Check_RequirementForTalentEffects();
    void  Set_LadderCheck(_bool LadderCheck) { m_bLadderCheck = LadderCheck; }
    _bool Get_LadderCheck() { return m_bLadderCheck; }
    list<weak_ptr<CWeapon>> Get_Weapon() { return m_pWeapons; }
    //void Set_DissolveAmount(const _float& In_fAmount) { m_fDissolveAmount = In_fAmount; }
public:
    void Focus_Monster(weak_ptr<CGameObject> In_pMonster);
    void Release_Focus();
    _bool Get_Focused() const
    {
        return m_bIsFocused;
    }

    weak_ptr<CMonster>  Get_TargetMonster() 
    {
        return m_pTargetMonster;
    }

public:
    virtual void Set_DissolveAmount(const _float& In_fAmount) override { m_fDissolveAmount = In_fAmount; } 
    void Set_DissolveAmount(const _uint In_iDissolveMeshIndex,const DISSOLVE_DESC In_DissolveDesc)
    {
        m_DissolveDescs[In_iDissolveMeshIndex] = In_DissolveDesc;
    }

public:
    FDelegate<_float>       Callback_UseSkill;

protected:
    virtual void SetUp_ShaderResource() override;
    virtual void SetUp_Requirement();

public:
    virtual void OnBattleEnd() override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;
    

public:
    virtual void    Save_ClientComponentData() { }
    virtual void    Load_ClientComponentData() { }



protected:
    virtual void OnEnable(void* pArg = nullptr) override;
    virtual void OnDisable() override;

public:
    virtual void Call_WeaponFirstAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void Call_WeaponAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

public:
    _flag           Get_CollisionObjectFlags() { return m_CollisionObjectFlags; }
    _flag           Get_CollisionMonstertFlags() { return m_CollisionMonsterFlags; }
    void            Set_CollisionObjectFlag(const _flag In_CollisionObjectFlags, const _bool In_bEnable);
 
protected:
    weak_ptr<CMonster> m_pTargetMonster;
    weak_ptr<CStateBase> m_pStandState;

    //list<weak_ptr<CCorvus_DefaultSaber>> m_pDefaultSaber;
    //list<weak_ptr<CCorvus_DefaultDagger>> m_pDefaultDagger;

    list<weak_ptr<CWeapon>> m_pWeapons;

    list<weak_ptr<CTalent_Effect>> m_pTalent_Effects;
    _flag                   m_iBindedTalentEffectes = 0;


    unordered_map<_uint, DISSOLVE_DESC>  m_DissolveDescs;
    _uint m_iNumMeshContainers = 0;


    weak_ptr<CGameObject> m_pMonster;
    weak_ptr<CTransform> m_pMonsterTransform;
    _bool               m_bIsFocused = false;


    //Created by Moon
protected:
    weak_ptr<CInventory>    m_pInventory;

private:
    _float              m_fNearSearchDelay = 0.f; 
    weak_ptr<CPlayer>   m_thisToPlayer;

protected:
    _flag      m_CollisionObjectFlags = 0;
    _flag      m_CollisionMonsterFlags = 0;
    _bool      m_bLadderCheck = false;
    string     m_szName;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

private:
    void Free();


};

END