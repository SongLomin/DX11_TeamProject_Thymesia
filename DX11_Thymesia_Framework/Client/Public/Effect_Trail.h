#pragma once
#include "EffectObject.h"

BEGIN(Engine)
class CVIBuffer_Trail;
class CBoneNode;
END

BEGIN(Client)

class CEffect_Trail :
    public CEffectObject
{
    GAMECLASS_H(CEffect_Trail)
    SHALLOW_COPY(CEffect_Trail)
    CLONE_H(CEffect_Trail, CGameObject)

public:
    void Set_TimeScaleLayer(const _uint& In_iTimeScaleLayer) {
        m_iTimeScaleLayerIndex = In_iTimeScaleLayer;
    }

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Update(_float fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);
    void Set_OwnerDesc(weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);
    void Set_TextureIndex(_uint In_iDiffuseIndex,_uint In_iMaskIndex, _uint In_iNoiseIndex)
    {
        m_iDiffuseIndex = In_iDiffuseIndex;
        m_iMaskIndex = In_iMaskIndex;
        m_iNoiseIndex = In_iNoiseIndex;
    }
protected:
    virtual void SetUp_ShaderResource() override;

protected:
    _uint                               m_iTimeScaleLayerIndex;
    weak_ptr<CVIBuffer_Trail>  m_pVIBuffer;
    weak_ptr <CTransform> m_pOwnerTransform;
    weak_ptr<CBoneNode> m_pOwnerBoneNode;
    weak_ptr<MODEL_DATA> m_pOwnerModel_Data;

    weak_ptr<CTexture> m_pMaskTextureCom;
    weak_ptr<CTexture> m_pNoiseTextureCom;

    _uint m_iDiffuseIndex = 0;
    _uint m_iMaskIndex =0;
    _uint m_iNoiseIndex=0;


   
public:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

private:

    void Free();

};

END