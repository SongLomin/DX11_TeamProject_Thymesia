#pragma once
#include "Actor.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
class CPhysXController;
END

BEGIN(Client)
class CStateBase;
class CStatus;

//class CBehaviorBase;

class CCharacter :
    public CActor
{
    GAMECLASS_H(CCharacter);
    CLONE_H(CCharacter, CGameObject);

public:
    // CGameObject��(��) ���� ��ӵ�
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;

public:
    weak_ptr<CCollider> Get_RigidColliderComponent() const;
    weak_ptr<CStateBase> Get_PreState() const;
    weak_ptr<CStateBase> Get_CurState() const;
    _uint Get_CurrentStateIndex() const;
    _uint Get_PreStateIndex() const;
    _bool Is_Edit() const { return m_isEdit; }
    
    weak_ptr<CStatus>   Get_Status() { return m_pStatus; }


    _uint Get_AttackCollisionLayer() const {
        return (_uint)m_eAttackCollisionLayer;
    };

    void Set_SuperArmor(const _bool In_bSuperArmor) { m_bSuperArmor = In_bSuperArmor; }
    _bool Get_SuperArmor() const { return m_bSuperArmor; }

public:
    void Set_RigidColliderEnable(const _bool& In_bEnable);

    virtual void Enable_Weapons(const _bool In_bEnable) {};

protected:
    virtual void Tick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;

protected:
    //weak_ptr<CBehaviorBase> m_pBehavior;
    weak_ptr<CCollider>     m_pHitColliderCom;
    weak_ptr<CCollider>     m_pRigidBodyColliderCom;
    weak_ptr<CNavigation>   m_pNaviMeshCom;
    weak_ptr<CStateBase>    m_pCurState;
    weak_ptr<CStateBase>    m_pPreState;
    weak_ptr<CStatus>       m_pStatus;
    weak_ptr<CPhysXController> m_pPhysXControllerCom;
    
    COLLISION_LAYER         m_eAttackCollisionLayer = COLLISION_LAYER::LAYER_END;

private:
    _bool m_isEdit = false;
    _bool     m_bSuperArmor = false;

public:
    virtual void OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage);
    virtual void Call_WeaponFirstAttack(weak_ptr<CCollider> pOtherCollider);
    virtual void Call_WeaponAttack(weak_ptr<CCollider> pOtherCollider);
protected:
    virtual void OnLevelEnter() override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

    virtual void OnEventMessage(_uint iArg) override;

private:
    void Free();
    
public: /* For Template Function */

    _bool Change_State(weak_ptr<CStateBase> pNextState, const _float& In_fAnimationBlendTime = 0.f);

    template<typename T>
    _bool Change_State(const _float& In_fAnimationBlendTime = 0.f)
    {
        static_assert(is_base_of<CStateBase, T>().value);

        weak_ptr<CStateBase> pNextState = Get_Component<T>();

        if (!pNextState.lock().get())
        {
            DEBUG_ASSERT;
            return false;
        }

        if (m_pCurState.lock().get())
        {
            m_pCurState.lock()->OnStateEnd();
            m_pCurState.lock()->Set_Enable(false);
        }

        m_pPreState = m_pCurState;
        m_pCurState = pNextState;

        m_pCurState.lock()->OnStateStart(In_fAnimationBlendTime);
        m_pCurState.lock()->Set_Enable(true);

        return true;

    }
};

END