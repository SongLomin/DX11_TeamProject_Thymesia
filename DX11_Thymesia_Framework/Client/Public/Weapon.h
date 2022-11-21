#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CBoneNode;
class CCollider;
class CVIBuffer_Trail;
END

BEGIN(Client)

class CWeapon :
    public CGameObject
{
    GAMECLASS_H(CWeapon);
    CLONE_H(CWeapon, CGameObject);

public:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Init_Weapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CGameObject> In_pParent, const string& szTargetNode = "WeaponCase1");
    void Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer);
    void Init_Trail(TRAIL_DESC& TrailDesc);
    void Enable_Weapon(const HIT_TYPE& In_eHitType, const _float& In_fDamage);
    void Disable_Weapon();
    void Set_WeaponScale(const _float& In_fWeaponScale);
    void Set_OriginalWeaponScale();
    weak_ptr<CGameObject> Get_ParentObject();

    void Set_WeaponDesc(const _float& In_fWeaponScale, const _float3& In_vOffset, const HIT_TYPE& In_eHitType, const _float& In_fDamage);
    _bool Set_TrailEnable(const _bool In_bEnable);
public:
    FDelegate<weak_ptr<CCollider>> CallBack_Attack;
    FDelegate<weak_ptr<CCollider>> CallBack_AttackOnce;


protected:
    void SetUp_ShaderResource();

protected:
    weak_ptr<class CEffect_Trail> m_pTrailEffect;
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CCollider> m_pHitColliderCom;

    weak_ptr<CGameObject> m_pParent;
    weak_ptr<CBoneNode> m_pTargetBoneNode;

    _float4x4				m_WorldMatrix;
    _float4x4               m_TransformationMatrix;

    _float3                 m_vOffset;

protected:
    list<_uint>             m_iHitColliderIndexs;
    HIT_TYPE                m_eHitType = HIT_TYPE::TYPE_END;
    _float                  m_fDamage = 0.f;
    _bool                   m_bFirstAttack = true;
    _float                  m_fWeaponScale = 1.f;
    _float                  m_fOriginalWeaponScale;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

public:
    virtual void OnSetDead() override;

    void Free();



};


END