#pragma once
#include "ClientComponent.h"
//#include "GameObject.h"

BEGIN(Engine)
class CAnimation;
class CNavigation;
class CTransform;
class CModel;
class CCollider;
class CPhysXController;

END

BEGIN(Client)

class CBehaviorBase;
class CCharacter;
class CStatus;


class CStateBase abstract:
    public CClientComponent
{
    GAMECLASS_H(CStateBase);

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);

    void Set_Animation(_uint In_iNumIndex, _float In_fFixedPlayRatio = -1.f);
    _uint Get_StateIndex()const { return m_iStateIndex; }
    _float3 Get_ShakingOffset();
    _flag   Get_StateFlag() const {return m_StateFlag;}

    weak_ptr<CCharacter> Get_OwnerCharacter() const;

public:
    virtual _bool Check_Requirement();
    virtual _bool Check_AndChangeNextState() PURE;
    virtual void Check_AndChangeHitState(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage);
    virtual void Init_Desc(void* In_pDesc) { }

public:
    _bool Get_NearGameObjectInDistance(weak_ptr<CGameObject>& Out_pGameObject, list<weak_ptr<CGameObject>> In_pGameObjects, const _float In_fDistance);

public:
    virtual void Call_OtherControllerHit(const PxControllersHit& In_hit) {};


protected:
    _flag m_StateFlag = 0;
    _uint m_iAnimIndex = 0;
    _uint m_iTimeScaleLayer = 0;
    _uint m_iStateIndex;
    _float m_fFixedPlayRatio = 0.f;
    _float3 m_vShakingOffSet = {0.f,0.f,0.f};

    //weak_ptr<CBehaviorBase> m_pBehavior;
    weak_ptr<CNavigation> m_pNaviCom;
    weak_ptr<CTransform> m_pTransformCom;
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CStatus> m_pStatusCom;
    weak_ptr<CPhysXController> m_pPhysXControllerCom;
    
private:
    weak_ptr<CCharacter>    m_pOwnerFromCharacter;
    static _uint            g_iStateIndex;
   

public: /* For. CallBack */
    FDelegate<> CallBack_StateStart;
    FDelegate<> CallBack_StateEnd;

public: /* For. EventFunction */
    virtual void OnDestroy() override;
    virtual void OnEnable(void* _Arg = nullptr) override;
    virtual void OnDisable() override;

    virtual void OnStateStart(const _float& In_fAnimationBlendTime);
    virtual void OnStateEnd();
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage);

public:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) {};
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) {};
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) {};

public:
    virtual void OnEventMessage(_uint iArg) override;
    virtual void OnEventMessage(weak_ptr<CBase> pArg);

protected:
    virtual void HitEffectSound();

protected:
    void Free();
    
};

END