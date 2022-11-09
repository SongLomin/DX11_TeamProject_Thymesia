#pragma once
#include "Character.h"

BEGIN(Client)

class CMonster;
class CPlayerStateBase;
class CCorvus_DefaultSaber;
class CCorvus_DefaultDagger;

class CPlayer :
    public CCharacter
{
    GAMECLASS_H(CPlayer);
    CLONE_H(CPlayer, CGameObject);

public:
    _bool Is_Warning() const { return m_bWarning; }

    void Set_TargetMonster(weak_ptr<CMonster> In_pMonster);
    weak_ptr<CMonster> Get_TargetMonster() const;

    _uint               Get_PlayerIndex() const {
        return m_iPlayerIndex;
    }

    _float              Get_DodgeTime() const {
        return m_fDodgeTime;
    }

    void              Set_DodgeTime(const _float& In_fDodgeTime) {
        m_fDodgeTime = In_fDodgeTime;
    }

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
    weak_ptr<CStateBase> m_pQTEState;
    _uint               m_iPlayerIndex = 0;

    list<weak_ptr<CCorvus_DefaultSaber>> m_pDefaultSaber;
    list<weak_ptr<CCorvus_DefaultDagger>> m_pDefaultDagger;

private:
    _float              m_fNearSearchDelay = 0.f;

private: /* For. Dodge */
    _bool               m_bWarning = false;
    _float              m_fDodgeTime = 0.f;
    _float              m_fDrawSkillBallRatio = 0.f;


protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

private:
    void Free();


};

END