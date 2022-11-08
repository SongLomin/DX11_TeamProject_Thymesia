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

public:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Init_Weapon(const WEAPON_DESC& In_WeaponDesc, weak_ptr<CTransform> In_ParentTransformCom);
    void Enable_Weapon(const _float& In_fLifeTime, const _bool& In_bSyncTransform);
    void Disable_Weapon();
    void Set_WeaponScale(const _float& In_fWeaponScale);
    weak_ptr<CCharacter> Get_ParentObject();

    //void Set_WeaponDesc(const _float& In_fLifeTime, const _bool& );

protected:
    void SetUp_ShaderResource();

private:
    void Update_TransformWithParent();

private:
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CCollider> m_pHitColliderCom;

    weak_ptr<CTransform> m_pParentTransformCom;
    weak_ptr<CCharacter> m_pParentCharacter;

private:
    WEAPON_DESC         m_tWeaponDesc;

    _float              m_fLifeTime = 0.f;
    _float              m_fCurrentFreq = 0.f;
    _bool               m_bSyncTransform = false;

private:
    list<_uint>             m_iHitColliderIndexs;
    _bool                   m_bFirstAttack = true;


protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

private:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

    void Free();



};

END