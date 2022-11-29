#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Client)

class CInteraction_Prop abstract :
    public CProp
{
    GAMECLASS_H(CInteraction_Prop);

public:
    enum PROP_ACT_TYPE 
    {
        ACT_ONEWAY_DOOR,
        ACT_LOCK_DOOR,
        ACT_LADDER_UP,
        ACT_LADDER_DOWN,

        PROP_ACT_END
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix) override;
       
public:
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

private:
    weak_ptr<CPhysXCollider>    m_pUse_PhyxXColliderCom;


    PROP_ACT_TYPE               m_eActType = PROP_ACT_TYPE::PROP_ACT_END;


public:
    void Free();
};

END