#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CVIBuffer_DynamicCube;
class CShader;

END


BEGIN(Client)

class CPhysXColliderObject;

class CEditSetActor final :
    public CGameObject
{
    GAMECLASS_H(CEditSetActor)
    CLONE_H(CEditSetActor, CGameObject)
    SHALLOW_COPY(CEditSetActor)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void  OnEventMessage(_uint iArg) override;
    virtual void  Write_Json(json& Out_Json) override;

private:
    void    View_CreateActor();
    void    View_Picking_Actor();
    void    View_Picking_List();
    void    View_SelectTransformInfo();
    void    View_Picking_Option();
    void    View_Picking_MessageEdit();

private:
    void    SetUp_ShaderResource_Select(ID3D11DeviceContext* pDeviceContext);

    void    Add_ActorToTool(_hashcode _HashCode, string _szTypeName, weak_ptr<CGameObject> _pInstance);
    _bool   Picking(RAY& _pMouseRayInWorldSpace, _float _fSetY);

private: 
    weak_ptr<CVIBuffer_DynamicCube>  m_pSelect_VIBufferCom;
    weak_ptr<CShader>                m_pSelect_ShaderCom;
    weak_ptr<CRenderer>              m_pRendererCom;
    
    _float4x4                        m_PickingDesc;
    _int                             m_iPickingIndex = 0;
    _bool                            m_bSubDraw      = false;

public:
    void Free();
};

END