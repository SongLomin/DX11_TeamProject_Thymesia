#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CModel;
class CCollider;
END

BEGIN(Client)

class CTrigger :
    public CGameObject
{
    GAMECLASS_H(CTrigger);

public:

    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

protected:
    virtual void SetUp_ShaderResource();

protected:
    weak_ptr<CCollider> m_pTriggerColliderCom;


protected:
    RENDERGROUP m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;
    _uint       m_iPassIndex = 0;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnEventMessage(_uint iArg) override;

private:
    void Free();

};

END