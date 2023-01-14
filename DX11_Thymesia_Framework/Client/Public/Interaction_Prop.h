#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Client)

class CInteraction_Prop abstract :
    public CProp
{
    GAMECLASS_H(CInteraction_Prop);

public:
    typedef enum Interaction_Type
    {
        INTERACTION_DOOR,
        INTERACTION_LADDER,
        INTERACTION_ELEVATOR,
        INTERACTION_ITEM,
        INTERACTION_NEXTPOINT,
        INTERACTION_DEADSPOT
    }INTERACTIONTYPE;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext) override;
	
public:
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

public:
    virtual void Act_Interaction();

protected:
    virtual HRESULT SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext);


protected:
    INTERACTIONTYPE             m_eInteractionType;

    FDelegate<_bool&>           CallBack_Requirement;
    list<_uint>                 m_CollisionIndex;

public:
    FDelegate<>                 Callback_ActStart;
    FDelegate<>                 Callback_ActEnd;
    FDelegate<_float, _bool&>   Callback_ActUpdate;

private:
    void Free();
};

END