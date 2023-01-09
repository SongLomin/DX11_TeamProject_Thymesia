#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

class CEffect_Decal final :
    public CEffectObject
{
    GAMECLASS_H(CEffect_Decal)
    SHALLOW_COPY(CEffect_Decal)
    CLONE_H(CEffect_Decal, CGameObject)

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    virtual void SetUp_ShaderResource();

private:
    _bool Check_IsDead();

private:
    weak_ptr<CVIBuffer_Cube> m_pVIBuffer;
    DECAL_DESC               m_DecalDesc;
    _float                   m_fTimeAcc = 0.f;

private:
    void Free();

};

