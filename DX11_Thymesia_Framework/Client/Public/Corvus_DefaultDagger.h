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

class CCorvus_DefaultDagger :
    public CGameObject
{
    GAMECLASS_H(CCorvus_DefaultDagger);
    CLONE_H(CCorvus_DefaultDagger, CGameObject);

public:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Init_DefaultDagger(weak_ptr<CModel> In_pModelCom, weak_ptr<CGameObject> In_pParent, const string& szTargetNode = "weapon_l");
    void Enable_DefaultDagger(const HIT_TYPE& In_eHitType, const _float& In_fDamage);
    void Disable_DefaultDagger();
    void Set_DefaultDaggerScale(const _float& In_fWeaponScale);
    void Set_OriginalDefaultDaggerScale();
    weak_ptr<CGameObject> Get_ParentObject();

    void Set_DefaultSaberDesc(const _float& In_fWeaponScale, const _float3& In_vOffset, const HIT_TYPE& In_eHitType, const _float& In_fDamage);

public:
    FDelegate<weak_ptr<CCollider>> CallBack_Attack;
    FDelegate<weak_ptr<CCollider>> CallBack_AttackOnce;


protected:
    void SetUp_ShaderResource();

private:
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CCollider> m_pHitColliderCom;

    weak_ptr<CGameObject> m_pParent;
    weak_ptr<CBoneNode> m_pTargetBoneNode;

    _float4x4				m_WorldMatrix;
    _float4x4               m_TransformationMatrix;

    _float3                 m_vOffset;

private:
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
    void Free();



};

END

