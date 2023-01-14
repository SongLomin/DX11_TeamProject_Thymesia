#pragma once
#include "BossMonster.h"


BEGIN(Client)

class CUrd :
    public CBossMonster
{
    GAMECLASS_H(CUrd);
    CLONE_H(CUrd, CGameObject);

public:
    virtual _float  Get_CamOffset() const override {
        return 0.5f;
    }
    void  Set_MoveScale(const _float3& In_vMoveScale) { m_vMoveScale = In_vMoveScale; }
    // 값변경이안되는게 컨스트고 레퍼런스 주소 8바이트 똑같다  12바이트  레퍼런스가이득 이해했다 값만써요 굿

    _uint  Get_CurrentDecoIndex() {
        return m_iDecoIndex;
    }
    list<weak_ptr<CJavelinWeapon>>    Get_JavelinWeapons() { return m_pJavelinWeapons; }
    list<weak_ptr<CMobWeapon>>      Get_DecoWeapons() { return   m_pDecoWeapons; }
private:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual void SetUp_ShaderResource() override;
    virtual void Init_Desc() override;

public:
    void Use_DecoWeapon();
    void Reset_Weapon();
 


private:
    weak_ptr<class CEffect_Trail>   m_pTrailEffect;
    list<weak_ptr<CJavelinWeapon>>  m_pJavelinWeapons;
    list<weak_ptr<CMobWeapon>>      m_pDecoWeapons;
    weak_ptr<CBoneNode> m_pTrailBoneNode;
    _float3             m_vMoveScale = {1.f,1.f,1.f};
    _uint               m_iDecoIndex = 0;

private:
    virtual void Move_RootMotion_Internal() override;
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnEventMessage(_uint iArg) override;
    //객체의 상태가 활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnEnable(void* _Arg = nullptr) override;

    //객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnDisable() override;

private:
    void Free();

};

END
