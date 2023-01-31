#pragma once
#include "Actor.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
END

BEGIN(Client)
class CStateBase;
class CStatus;
class CPhysXCharacterController;

//class CBehaviorBase;

class CCharacter :
    public CActor
{
    GAMECLASS_H(CCharacter);
    CLONE_H(CCharacter, CGameObject);

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;

public:
    weak_ptr<CStateBase> Get_PreState() const;
    weak_ptr<CStateBase> Get_CurState() const;
    weak_ptr<CPhysXController> Get_PhysXController() const;
    _uint Get_CurrentStateIndex() const;
    _uint Get_PreStateIndex() const;
    _bool Is_Edit() const { return m_isEdit; }
    PxControllerCollisionFlags  Get_LastCollisionFlags() const { return m_LastCollisionFlags; }
  

    weak_ptr<CPhysXCharacterController> Get_PhysX() { return m_pPhysXControllerCom; }
    weak_ptr<CStatus>   Get_Status() { return m_pStatus; }
	void   Set_Status(weak_ptr<CStatus> pStatusCom) {
        m_pStatus = pStatusCom;
	}

    void Set_Edit(const _bool In_IsEdit) { m_isEdit = In_IsEdit; }

    virtual void Set_DissolveAmount(const _float& In_fAmount) { m_fDissolveAmount = In_fAmount; }
    _uint Get_AttackCollisionLayer() const {
        return (_uint)m_eAttackCollisionLayer;
    };

    void Set_AttackCollisionLayer(const COLLISION_LAYER eCollisionLayer) {
        m_eAttackCollisionLayer = eCollisionLayer;
    }

    void Set_SuperArmor(const _bool In_bSuperArmor) { m_bSuperArmor = In_bSuperArmor; }
    _bool Get_SuperArmor() const { return m_bSuperArmor; }
    void Set_PassIndex(const _uint In_iPassIndex) { m_iPassIndex = In_iPassIndex; }
    void Set_RimLightDesc(const _float In_fLightTime, const _float3 In_vColor, const _float In_fPower,const EASING_TYPE In_eEasingType = EASING_TYPE::TYPE_END);

public:
    virtual void                        OnStealMonsterSkill(MONSTERTYPE Monstertype);


public:
    virtual void Enable_Weapons(const _bool In_bEnable) {};

protected:
    virtual void Tick(_float fTimeDelta) override;
    virtual void Thread_PreLateTick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    
protected:
    void            Move_RootMotion(_float fTimeDelta);
    virtual void    Move_RootMotion_Internal() {};


public:
    FDelegate<weak_ptr<CStateBase>> Callback_OnChangeState;

protected:
    weak_ptr<CCollider>     m_pHitColliderCom;
    weak_ptr<CNavigation>   m_pNaviMeshCom;
    weak_ptr<CStateBase>    m_pCurState;
    weak_ptr<CStateBase>    m_pPreState;
    weak_ptr<CStatus>       m_pStatus;
    weak_ptr<CPhysXCharacterController> m_pPhysXControllerCom;
    
    COLLISION_LAYER         m_eAttackCollisionLayer = COLLISION_LAYER::LAYER_END;

    _float m_fDissolveAmount = 0.f;

    string                  m_szClientComponentPath = "../Bin/ClientComponentData/";

    _float m_fRimLightTime = 1.f;
    _float m_fRimLightTimeAcc = 0.f;
    _float m_fRimLightPower = 0.f;
    EASING_TYPE m_eRimLightEasingType = EASING_TYPE::TYPE_END;
    _float3 m_vRimLightColor = { 0.f,0.f,0.f };

private:
    _bool m_isEdit = false;
    _bool     m_bSuperArmor = false;
    
protected:
    PxControllerCollisionFlags m_LastCollisionFlags;


protected:
    


public:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage);
    virtual void Call_WeaponFirstAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual void Call_WeaponAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual void OnEventMessage(_uint iArg) override;

protected:
    virtual void OnLevelEnter() override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

    

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

        Callback_OnChangeState(m_pCurState);


        m_pCurState.lock()->Set_Enable(true);

        return true;

    }
};

END