#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Curve;
class CShader;
class CRenderer;
END

BEGIN(Client)

class CTargetCurve :
    public CGameObject
{
    GAMECLASS_H(CTargetCurve)
    CLONE_H(CTargetCurve, CGameObject)
    SHALLOW_COPY(CTargetCurve)

private:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void SetUp_ShaderResource();

public:
    // ex. player
    void Init_ParentCurve(weak_ptr<CTransform> pParentTransform);
    void Set_Target(weak_ptr<CTransform> pTargetTransform);

public:
    void Call_UpdateTarget();
    void Call_ReleaseTarget();

private: /* My Components */
    weak_ptr<CVIBuffer_Curve>   m_pVIBufferCom;
    weak_ptr<CShader>           m_pShaderCom;
    weak_ptr<CRenderer>         m_pRenderderCom;

private: /* ParentTransform */
    weak_ptr<CTransform>        m_pParentTransformCom;
    weak_ptr<CTransform>        m_pTargetTransformCom;
    weak_ptr<CModel>            m_pTargetModelCom;
    weak_ptr<CBoneNode>         m_pTargetBoneNodeCom;

private:
    _float4x4                   m_CurvePoints;

private:
    virtual void OnDestroy() override;
    void Free();
};

END