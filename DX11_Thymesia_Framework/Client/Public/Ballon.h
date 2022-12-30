#pragma once
#include "NorMonster.h"


BEGIN(Client)

class CMonsterHPBar_Base;

class CBallon :
    public CNorMonster
{
    GAMECLASS_H(CBallon);
    CLONE_H(CBallon, CGameObject);

public:
    virtual _float  Get_CamOffset() const override {
        return 0.5f;
    }

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual void SetUp_ShaderResource() override;

public:
    virtual void Respawn_Monster(_fvector In_vPosition) override;

    virtual void Init_Desc() override;

private:
    virtual void Move_RootMotion_Internal() override;

private:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

    virtual void OnEventMessage(_uint iArg) override;

    virtual void OnEnable(void* _Arg = nullptr) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

private:
    void Free();

};

END
