#pragma once
#include "Attack_Area.h"
#include "Client_Defines.h"

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
    public CAttackArea
{
    GAMECLASS_H(CWeapon);
    CLONE_H(CWeapon, CGameObject);

protected:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Init_Weapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode = "WeaponCase1");
    void Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer);
    void Init_Trail(TRAIL_DESC& TrailDesc);
    void Add_Collider(_fvector In_vOffset, const _float In_fScale, const COLLISION_LAYER In_Layer);
    void Enable_Weapon();
    void Disable_Weapon();
    weak_ptr<CCharacter> Get_ParentCharacter();

    void Set_WeaponDesc(const WEAPON_DESC& In_Weapon);
    void Set_WeaponDesc(const HIT_TYPE In_eHitType, const _float In_fDamage, const ATTACK_OPTION In_eOptionType = ATTACK_OPTION::OPTION_END);
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
    
    weak_ptr<CBoneNode> m_pTargetBoneNode;

    _float4x4				m_WorldMatrix;
    _float4x4               m_TransformationMatrix;
    _float3                 m_vOffset;

protected:
    WEAPON_DESC             m_tWeaponDesc;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

public:
    virtual void OnSetDead() override;

    void Free();



};


END