#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CCollider;
class CPhysXCollider;

END

BEGIN(Client)

class CProp_Fence final :
    public CProp
{
    GAMECLASS_H(CProp_Fence);
    CLONE_H(CProp_Fence, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    virtual HRESULT SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext);

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    Act_Delay(_float _fTimeDelta, _bool& _bEnd);
    void    Act_Event(_float _fTimeDelta, _bool& _bEnd);

private:
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;
    weak_ptr<CTexture>          m_pDissolveTextureCom;

    _float                      m_fDelayTime     = 0.f;
    _float                      m_fDissolveSpeed = 1.f;
    _int                        m_iSectionIndex  = -1;

    FDelegate<_float, _bool&>   Callback_ActEvent;
    _float                      m_fDissolveRatio = 0.f;
    _float                      m_fAccTime       = 0.f;
    _bool                       m_bDelayDone     = false;
    _float4                     m_vFirstPosition = { 0.f, 0.f, 0.f, 1.f };

private:
    virtual void OnDestroy() override;
    void Free();
};

END

