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

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Update(_float fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);
    void Set_OwnerDesc(weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);
   
protected:
    virtual void SetUp_ShaderResource() override;

private:
    _uint                               m_iTimeScaleLayerIndex;
    weak_ptr<CVIBuffer_Trail>  m_pVIBuffer;
    weak_ptr <CTransform> m_pOwnerTransform;
    weak_ptr<CBoneNode> m_pOwnerBoneNode;
    weak_ptr<MODEL_DATA> m_pOwnerModel_Data;

   
public:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

private:

    void Free();

};

END