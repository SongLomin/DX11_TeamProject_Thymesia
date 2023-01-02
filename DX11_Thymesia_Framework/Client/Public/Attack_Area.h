#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CBoneNode;
class CCollider;
END

BEGIN(Client)

class CCharacter;

class CAttackArea :
    public CGameObject
{
    GAMECLASS_H(CAttackArea);
    CLONE_H(CAttackArea, CGameObject);
    SHALLOW_COPY(CAttackArea);

public:
    HIT_TYPE Get_HitType() const;
    ATTACK_OPTION Get_OptionType() const;

public:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Init_AttackArea(const ATTACKAREA_DESC& In_WeaponDesc, weak_ptr<CTransform> In_ParentTransformCom);
    void Enable_Weapon(const _float& In_fLifeTime);
    void Disable_Weapon();
    weak_ptr<CCharacter> Get_ParentObject();

    //void Set_WeaponDesc(const _float& In_fLifeTime, const _bool& );

protected:
    void Update_TransformWithParent();
    _bool Check_AllDisableCollider();

protected:
    list<weak_ptr<CCollider>> m_pHitColliderComs;

    weak_ptr<CTransform> m_pParentTransformCom;
    weak_ptr<CCharacter> m_pParentCharacter;

    list<_uint>             m_iHitColliderIndexs;
    _bool                   m_bFirstAttack = true;

protected:
    ATTACKAREA_DESC         m_tWeaponDesc;

    _float                  m_fLifeTime = 0.f;
    _float                  m_fCurrentFreq = 0.f;

    _uint                   m_iTimeScaleLayer = (_uint)TIMESCALE_LAYER::NONE;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

private:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

    void Free();



};

END