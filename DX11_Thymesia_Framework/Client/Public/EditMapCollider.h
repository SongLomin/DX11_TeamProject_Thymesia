#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPhysXColliderObject;

class CEditMapCollider final :
    public CGameObject
{
    GAMECLASS_H(CEditMapCollider)
    CLONE_H(CEditMapCollider, CGameObject)
    SHALLOW_COPY(CEditMapCollider)

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
    void    View_Picking_Collider();
    void    View_Picking_List();
    void    View_SelectTransformInfo();
    void    View_Picking_Option();

    void    View_Act_DeleteSame();

private: 
    _float4x4               m_PickingDesc;
    _int                    m_iPickingIndex = 0;
    _uint                   m_iOption       = 0;
    _bool                   m_bRender       = false;

public:
    void Free();
};

END