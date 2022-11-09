#pragma once
#include "Player.h"

BEGIN(Client)

class CCamera_Target;

class CCorvus : 
    public CPlayer
{
    GAMECLASS_H(CCorvus);
    CLONE_H(CCorvus, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void SetUp_ShaderResource() override;

private:
    weak_ptr<CCamera_Target> m_pCamera;
    weak_ptr<CTransform> m_pCameraTransform;


protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

private:
    virtual void OnBattleEnd() override;
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

    void Free();

};

END