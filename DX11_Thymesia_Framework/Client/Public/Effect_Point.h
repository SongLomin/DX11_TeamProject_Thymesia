#pragma once
#include "EffectObject.h"

BEGIN(Engine)
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CEffect_Point :
    public CEffectObject
{
    GAMECLASS_H(CEffect_Point)
    SHALLOW_COPY(CEffect_Point)
    CLONE_H(CEffect_Point, CGameObject)

public:
    void Set_TimeScaleLayer(const _uint& In_iTimeScaleLayer) {
        m_iTimeScaleLayerIndex = In_iTimeScaleLayer;
    }

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Init_EffectParticle(const _char* In_szTextureKey);

protected:
    virtual void SetUp_ShaderResource() override;

private:
    _uint                               m_iTimeScaleLayerIndex;
    weak_ptr<CVIBuffer_Point_Instance>  m_pVIBuffer;

private:
    void Free();

};

END