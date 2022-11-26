#pragma once
#include "Player.h"

BEGIN(Client)

class CCamera_Target;

class CCorvus : 
    public CPlayer
{
    GAMECLASS_H(CCorvus)
    CLONE_H(CCorvus, CGameObject)

public:
    virtual HRESULT Initialize_Prototype()         override;
    virtual HRESULT Initialize(void* pArg)         override;
    virtual HRESULT Start()                        override;
    virtual void Tick(_float fTimeDelta)           override;
    virtual void LateTick(_float fTimeDelta)       override;
    virtual void Before_Render(_float fTimeDelta)  override;
    virtual void Custom_Thread1(_float fTimeDelta) override;
    virtual HRESULT Render()                       override;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider)  override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider)  override;

private:
    void Ready_Weapon();
    void Ready_States();
    virtual void SetUp_ShaderResource() override;
    void RootMove();
    virtual void OnBattleEnd() override;
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;
    void Free();

private:
    weak_ptr<CCamera_Target> m_pCamera;
    weak_ptr<CTransform> m_pCameraTransform;

    _uint m_iNumMeshContainers = 0;
};

END