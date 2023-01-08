#pragma once
#include "MobWeapon.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CBoneNode;
class CCollider;
END

BEGIN(Client)

class CUrdWeapon final :
    public CMobWeapon
{
public:
    GAMECLASS_H(CUrdWeapon);
    CLONE_H(CUrdWeapon, CGameObject);

public:
    void          Set_WeaponNum(_uint iWeaponNum) { m_iWeaponNum = iWeaponNum; }
    _uint         Get_WeaponNum() { return m_iWeaponNum; }
    void          Set_UsingCheck(_bool bUsingCheck) { m_bUsingCheck = bUsingCheck; }
    _bool         Get_UsingCheck() { return m_bUsingCheck; }

public:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Thread_PreBeforeRender(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    _bool Set_TrailEnable(const _bool In_bEnable);

private:
    weak_ptr<class CEffect_Trail> m_pTrailDistortion;
    weak_ptr<class CEffect_Trail> m_pTrailDiffuse;
    _uint                         m_iWeaponNum = 0;
    _bool                         m_bUsingCheck = false;
    
private:
    void SetUp_ShaderResource();



private:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

public:
    void Free();
};

END

