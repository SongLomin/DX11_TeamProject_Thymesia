#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CPhysXCollider;
END


BEGIN(Client)

class CStatic_Prop final :
    public CProp
{
    GAMECLASS_H(CStatic_Prop);
    CLONE_H(CStatic_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix) override;

private:
    weak_ptr<CPhysXCollider> m_pPhyxXColliderCom;

public:
    void Free();
};

END