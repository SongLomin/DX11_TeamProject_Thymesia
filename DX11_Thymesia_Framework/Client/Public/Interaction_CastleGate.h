#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CCollider;
class CPhysXCollider;

END

BEGIN(Client)

class CInteraction_CastleGate final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_CastleGate);
    CLONE_H(CInteraction_CastleGate, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    
private:
    void Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd);
    void Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd);

    void Requirement_Key(_bool& Out_bRequirement);

private:
    HRESULT SetUp_ShaderResource_Default(ID3D11DeviceContext* pDeviceContext);
    HRESULT DrawShader_Body(ID3D11DeviceContext* pDeviceContext);
    HRESULT DrawShader_Door(ID3D11DeviceContext* pDeviceContext);
    HRESULT DrawShader_Gear(ID3D11DeviceContext* pDeviceContext);

public:
    virtual void Act_Interaction() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer);

private:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CPhysXCollider>    m_pRightPhysXColliderCom;
    weak_ptr<CPhysXCollider>    m_pLeftPhysXColliderCom;
    weak_ptr<CModel>            m_pDoorRightModelCom;
    weak_ptr<CModel>            m_pDoorLeftModelCom;
    weak_ptr<CModel>            m_pGearModelCom;
         
    _float3                     m_vOffset             = { 0.f, 0.f, 0.f };
    _float                      m_fDoorOffset         = -4.8f;
                                                      
    _bool                       m_bActionFlag         = true;
    _float                      m_fGearRotationRadian = 0.f;
    _float                      m_fDoorRotationRadian = 0.f;
    
    ITEM_NAME                   m_iKeyID              = ITEM_NAME::ITEM_NAME_END;
                                                      
private:
    void Free();
};

END

