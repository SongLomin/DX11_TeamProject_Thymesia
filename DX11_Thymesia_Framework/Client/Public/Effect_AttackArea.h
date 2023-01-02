#pragma once
#include "Attack_Area.h"

BEGIN(Client)

class CEffectGroup;
class CCustomEffectMesh;

class CEffect_AttackArea :
    public CAttackArea
{
    GAMECLASS_H(CEffect_AttackArea);
    CLONE_H(CEffect_AttackArea, CGameObject);
    SHALLOW_COPY(CEffect_AttackArea);

public:
    void Init_EffectAttackArea(const EFFECT_ATTACKAREA_DESC& In_Desc, weak_ptr<CEffectGroup> pEffectGroup, weak_ptr<CCustomEffectMesh> pEffectMesh);

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

private:
    void Update_TransformWithEffectMesh();

private:
    EFFECT_ATTACKAREA_DESC m_Desc;
    weak_ptr<CEffectGroup> m_pEffectGroup;
    weak_ptr<CCustomEffectMesh> m_pEffectMesh;

private:
    void Free();

};

END