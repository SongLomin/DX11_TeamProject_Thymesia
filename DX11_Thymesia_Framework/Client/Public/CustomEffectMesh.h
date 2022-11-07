#pragma once
#include "EffectMesh.h"


BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CSkill;
class CAttackArea;

class CCustomEffectMesh :
    public CEffectMesh
{
    GAMECLASS_H(CCustomEffectMesh)
        SHALLOW_COPY(CCustomEffectMesh)
        CLONE_H(CCustomEffectMesh, CGameObject);

public:
    const _char* Get_EffectName() const;
    // 이펙트를 끝낼 수 있는지 여부. 루핑 이펙트는 다르게 처리한다.
    _bool Is_Finish() const;
    void Set_TimeScaleLayer(const _uint& In_iTimeScaleLayer) {
        m_iTimeScaleLayerIndex = In_iTimeScaleLayer;
    }

protected:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void SetUp_ShaderResource() override;

public:
    void Reset_Effect();
    void Reset_Effect(weak_ptr<CTransform> pParentTransform);
    
public: /* For. Editer */
    void Init_EffectMesh(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey);
    void Sync_Animation();
    void Play(_float fTimeDelta);
    

    void Write_FromBinary(ostream& os);
    void Load_FromBinary(istream& is);
    void Write_EffectJson(json& Out_Json);
    void Load_EffectJson(const json& In_Json, const _uint& In_iTimeScaleLayer);

private:
    void Play_Internal(_float fFrameTime);

#ifdef _DEBUG
    void Clone_EffectMesh();


#endif // _DEBUG


private:
    weak_ptr<CAttackArea> m_pAttackArea;
    weak_ptr<CTransform> m_pParentTransformCom;

private:
    _uint           m_iTimeScaleLayerIndex;
    _bool           m_bFinish;
    _float4x4       m_ParentMatrix;
    _float          m_fPreFrame;

private:
    string          m_szEffectName;
    EFFECTMESH_DESC m_tEffectMeshDesc;

    _float2 m_vUv;
    weak_ptr<CTexture> m_pMaskTextureCom;
    weak_ptr<CTexture> m_pGradientTextureCom;

    _float          m_fCurrentLifeTime = 0.f;
    _float          m_fCurrentInitTime = 0.f;

    // For. Position
    _float3         m_vCurrentSpeed;
    _float3         m_vCurrentForce;

    // For. Rotation
    _float3         m_vCurrentRotation;
    _float3         m_vCurrentRotationForce;

    // For. Scale
    _float3         m_vCurrentScale;
    _float3         m_vCurrentScaleSpeed;
    _float3         m_vCurrentScaleForce;

    // For. Color
    _float4         m_vCurrentColor;
    _float4         m_vCurrentColorForce;

    // For. UV
    _float2         m_vCurrentUV;
    _float2         m_vCurrentUVForce;

    // For. Glow
    _float4         m_vCurrentGlowColor;
    _float4         m_vCurrentGlowForce;


public:
    virtual void OnEventMessage(_uint iArg) override;

protected:
    //객체의 상태가 활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnEnable(void* _Arg = nullptr) override;

    //객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

public:
    void OnChangeAnimationKey(const _uint& In_Key);

private:
    virtual void Free() override;

};

END