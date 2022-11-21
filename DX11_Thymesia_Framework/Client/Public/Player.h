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
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix) override;


public:
    void Search_NearTargetMonster(_float fTimeDelta);
    void Forced_SearchNearTargetMonster();

protected:
    virtual void SetUp_ShaderResource() override;

public:
    virtual void OnBattleEnd() override;
    
protected:
    virtual void OnEnable(void* pArg = nullptr) override;
    virtual void OnDisable() override;

public:
    virtual void Call_WeaponFirstAttack(weak_ptr<CCollider> pOtherCollider) override;
    virtual void Call_WeaponAttack(weak_ptr<CCollider> pOtherCollider) override;

protected:
    weak_ptr<CMonster> m_pTargetMonster;
    weak_ptr<CStateBase> m_pStandState;
    
    weak_ptr<CPhysXCollider> m_pPhysXColliderCom;

    //list<weak_ptr<CCorvus_DefaultSaber>> m_pDefaultSaber;
    //list<weak_ptr<CCorvus_DefaultDagger>> m_pDefaultDagger;

    list<weak_ptr<CWeapon>> m_pWeapons;

private:
    _float              m_fNearSearchDelay = 0.f;

public:
  



protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

private:
    void Free();


};

END