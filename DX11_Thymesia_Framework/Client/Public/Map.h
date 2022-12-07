#pragma once
#include "Prop.h"

BEGIN(Engine)
class CNavigation;
END


BEGIN(Client)
class CMap :
    public CProp
{
    GAMECLASS_H(CMap);
    CLONE_H(CMap, CGameObject);

public:

    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix) override;

protected:
    virtual HRESULT SetUp_ShaderResource() override;

protected:
    weak_ptr<CNavigation> m_pNaviMeshCom;

protected:
    void Free();

};

END