#pragma once
#include "BossMonster.h"

BEGIN(Engine)
class CBoneNode;
END

BEGIN(Client)
class CMonsterHPBar_Boss;
class CVarg :
    public CBossMonster
{
    GAMECLASS_H(CVarg);
    CLONE_H(CVarg, CGameObject);

public:
    virtual _float  Get_CamOffset() const override { return 0.5f; }

public:
    // CGameObject��(��) ���� ��ӵ�
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Thread_PreBeforeRender(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual void SetUp_ShaderResource() override;
    void Set_TrailEnable(_bool In_bEnable);
    void Set_EyeTrailEnable(_bool In_bEnable);
    virtual void Init_Desc() override;

private:
    weak_ptr<class CEffect_Trail> m_pTrailEffect;
    weak_ptr<CBoneNode> m_pTrailBoneNode;

    _float4x4                     m_TransformationMatrix;

private:
    virtual void Move_RootMotion_Internal() override;
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnEventMessage(_uint iArg) override;
    //��ü�� ���°� Ȱ��ȭ ���·� ����� ��, ȣ��Ǵ� �̺�Ʈ�Դϴ�.
    virtual void OnEnable(void* _Arg = nullptr) override;
    //��ü�� ���°� ��Ȱ��ȭ ���·� ����� ��, ȣ��Ǵ� �̺�Ʈ�Դϴ�.
    virtual void OnDisable() override;
    void Free();
};

END
