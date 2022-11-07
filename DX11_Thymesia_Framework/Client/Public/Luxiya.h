#pragma once
#include "Player.h"
BEGIN(Client)

class CSkill;
class CLuxiyaBlueSkillCharacter;

class CLuxiya :
    public CPlayer
{
    GAMECLASS_H(CLuxiya);
    CLONE_H(CLuxiya, CGameObject);

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void SetUp_ShaderResource() override;

public:
    void Set_BlueSkillState(const _float& In_fBlueSkillTime);
    virtual void Call_WeaponFirstAttack(weak_ptr<CCollider> pOtherCollider) override;
    void    Call_LuxiyaTalk();

private:
    void Update_BlueSkillEffect(_float fTimeDelta);

private: /* For. Default Effect */
    
    
private: /* For. BlueSkill */
    list<weak_ptr<CLuxiyaBlueSkillCharacter>> m_pBlueSkillEffects;

    _float                      m_fBlueSkillTime = 0.f;
    _float                      m_fBlueSkillFreq = 0.08f;
    _float                      m_fCurrentBlueSkillFreq = 0.f;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

private:
    virtual void OnBattleEnd() override;
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

    virtual void Free() override;

};

END