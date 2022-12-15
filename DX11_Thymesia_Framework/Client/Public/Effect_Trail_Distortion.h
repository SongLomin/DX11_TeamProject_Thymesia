#pragma once
#include "Effect_Trail.h"

BEGIN(Client)

class CEffect_Trail_Distortion :
    public CEffect_Trail
{
    GAMECLASS_H(CEffect_Trail_Distortion)
    SHALLOW_COPY(CEffect_Trail_Distortion)
    CLONE_H(CEffect_Trail_Distortion, CGameObject)

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    virtual void SetUp_ShaderResource() override;

   
public:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

private:

    void Free();

};

END