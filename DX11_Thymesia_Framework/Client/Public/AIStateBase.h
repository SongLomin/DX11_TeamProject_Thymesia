#pragma once
#include "StateBase.h"

BEGIN(Engine)
class CAnimation;
class CNavigation;
END

BEGIN(Client)

class CMonster;
class CAIStateBase abstract:
    public CStateBase
{
    GAMECLASS_H(CAIStateBase);

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

public:
    virtual void            Init_Desc(void* In_pDesc) override;
    virtual _bool           Check_RequirementCoolDown(weak_ptr<CAIStateBase> pTargetState, const _float& In_fCoolTime);

public:
    _float                  Get_StateTimeAcc() const { return m_fStateTimeAcc; }
    shared_ptr<CMonster>    Get_OwnerMonster() const noexcept;
    virtual _bool           Check_Requirement() override;

protected:
    void                    Set_AtkPermitAndNoHurt(_bool AtkPermitAndNoHurt) { m_bAtkPermitAndNoHurt = AtkPermitAndNoHurt; }
    void                    Turn_ToThePlayer(_float fTimeDelta);
    _vector                 Get_InputToLookDir();
    void                    StartPositonLookAt(_float fTimeDelta);
    _bool                   Rotation_InputToLookDir();
    _bool                   Rotation_TargetToLookDir();
    _float                  Get_DistanceWithPlayer() const;
    _bool                   Check_RequirementIsTargeted();
   
    virtual void HitEffectSound() override;

public:
    virtual void           Call_OtherControllerHit(const PxControllersHit& In_hit) override;
  
protected:
    _float3                m_vLookAtDir;
    _float                 m_fCurrentRotateTime = 0.f;
    _float                 m_fStateTimeAcc = 100.f;
    MONSTERTYPE            m_eMonType =     MONSTERTYPE::TYPE_END; // 현재 몬스터가 어떤몬스터타입인지
    NORMONSTERIDLETYPE     m_eNorMonIdleType = NORMONSTERIDLETYPE::IDLEEND; // 현재몬스터가 어떤아이들타입인지
    weak_ptr<CTransform>   m_pTransformCom;  // 몬스터 트랜스폼받아오고저장할려고만듬
    _float4                m_fStartPosition;
    _bool                  m_bPatrol = false;
                           
    //TODO 보스로 빼요      
    BOSSSTARTTYPE          m_eBossStartType = BOSSSTARTTYPE::BOSSSTARTEND;
                 

    //TODO 임시
protected:                 
    _bool                  m_bAttackLookAtLimit = false; // 공격할떄룩엣하다가 찍을떄쯤 풀라고
    _bool                  m_bAtkPermitAndNoHurt = false;

protected:
    _vector                Get_CurMonToStartMonDir();
    _float                 GetStartPositionToCurrentPositionDir();
    virtual void           TurnMechanism();
    _float                 ComputeAngleWithPlayer();
    _int                   ComputeDirectionToPlayer();
    _int                   Compute_DirectionToOtherPosition(const _float3& In_OtherPos);

private:
    weak_ptr<CMonster>     m_pOwnerFromMonster;

public: /* For. EventFunction */
    virtual void          OnDestroy() override;
    virtual void          OnEnable(void* _Arg = nullptr) override;
    virtual void          OnDisable() override;
                          
    virtual void          OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void          OnStateEnd() override;

protected:
    void Free();

};

END