#pragma once
#include "EffectObject.h"

BEGIN(Engine)
class CVIBuffer_Rect_Instance;
class CTransform;
class CBoneNode;
class CModel;
END

BEGIN(Client)
class CEffect_Rect final :
    public CEffectObject
{
    GAMECLASS_H(CEffect_Rect)
    SHALLOW_COPY(CEffect_Rect)
    CLONE_H(CEffect_Rect, CGameObject)

private:
    enum class PARTICLETYPE
    {
        NONE,
        OUTBURST,
        ATTRACTION,
        BILLBOARD,
        TYPE_END
    };

    enum class TRANSFORMTYPE
    {
        STATIC,
        JUSTSPAWN,
        CHILD,
        TYPE_END
    };

public:
    void Set_TimeScaleLayer(const _uint& In_iTimeScaleLayer) { m_iTimeScaleLayerIndex = In_iTimeScaleLayer; }
    const _char* Get_EffectName() const;

    void Init_EffectParticle(const _char* In_szName, const _char* In_szTextureKey);
    void Reset_Effect(weak_ptr<CTransform> pParentTransform);
    void Sync_Animation();
    void UnUse_EffectParticle();
    void ReBake_EditParticle();
    void Write_EffectJson(json& Out_Json);
    void Load_EffectJson(const json& In_Json, const _uint& In_iTimeScaleLayer);
    virtual void OnEventMessage(_uint iArg) override;
    void OnChangeAnimationKey(const _uint& In_Key);

#ifdef _DEBUG
    const void Trigger_Reset(weak_ptr<CTransform> pPreviewModelTransform) 
    {
        m_pParentTransformCom = pPreviewModelTransform;
        m_bResetTrigger = true;
    }
#endif // _DEBUG

private:
    virtual HRESULT Initialize_Prototype()   override;
    virtual HRESULT Initialize(void* pArg)   override;
    virtual void Tick(_float fTimeDelta)     override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext)                 override;

    virtual void SetUp_ShaderResource();

    void Play(_float fTimeDelta);
    void Reset_Instance(const _uint& In_ParticleCount);
    void Reset_ParticleDescs();
    void Reset_ParticleDesc(const _uint& In_iIndex);
    void Generate_RandomOriginalParticleDesc();
    _bool Check_DisableAllParticle();

    void Play_Internal(const _uint& i, _float fTimeDelta);
    void Update_ParticlePosition(const _uint& i, _float fTimeDelta);
    void Update_ParticleRotation(const _uint& i, _float fTimeDelta);
    void Update_ParticleScale(const _uint& i, _float fTimeDelta);
    void Update_ParticleUV(_float fTimeDelta);
    void Update_ParticleColor(const _uint& i, _float fTimeDelta);
    void Update_ParticleGlowColor(_float fTimeDelta);
    void Update_ParticleSpriteFrame(const _uint& i, _float fTimeDelta);
    void Update_ParentTransform();

    void Apply_Easing
    (
        _float3& vTarget
        , EASING_TYPE eEasingType
        , _vector vStartPoint
        , _vector vTargetPoint
        , _float  fElapsedTime
        , _float  fTotalTime
    );

    void Apply_Easing
    (
        _float2& vTarget
        , EASING_TYPE eEasingType
        , _vector vStartPoint
        , _vector vTargetPoint
        , _float  fElapsedTime
        , _float  fTotalTime
    );

    void Apply_Easing
    (
        _float& fTarget
        , EASING_TYPE eEasingType
        , _float fStartPoint
        , _float fTargetPoint
        , _float fElapsedTime
        , _float fTotalTime
    );

    const _bool Is_Sprite() const;

#ifdef _DEBUG
private: // For. Tool
#ifdef _JOJO_EFFECT_TOOL_
    void Show_ShaderPasses();
#endif // _JOJO_EFFECT_TOOL_
    void Tool_Spawn_Life_Time();
    void Tool_Boner();
    void Tool_Position();
    void Tool_Position_Easing();
    void Tool_Speed();
    void Tool_Speed_Easing();
    void Tool_Rotation();
    void Tool_Rotation_Easing();
    void Tool_Scale();
    void Tool_Scale_Easing();
    void Tool_Sprite();
    void Tool_Color();
    void Tool_Color_EasingAlpha();
    void Tool_Texture_Diffuse();
    void Tool_Texture_Mask();
    void Tool_Texture_Noise();
    void Tool_Glow();
#endif // _DEBUG
private:
    void Free();

private:
    weak_ptr<CTransform>                m_pParentTransformCom;
    _float                              m_fPreFrame = 0.f;

    _uint                               m_iTimeScaleLayerIndex;
    _bool                               m_bFinish;
    _bool                               m_bStopParticle;

    weak_ptr<CVIBuffer_Rect_Instance>   m_pVIBuffer;

    weak_ptr<CTexture>                  m_pColorDiffuseTextureCom;
    _float2                             m_vDiffuseCurrentUV;
    _float2                             m_vDiffuseCurrentUVForce;

    weak_ptr<CTexture>                  m_pMaskTextureCom;
    _float2                             m_vMaskCurrentUV;
    _float2                             m_vMaskCurrentUVForce;

    weak_ptr<CTexture>                  m_pNoiseTextureCom;
    _float2                             m_vNoiseCurrentUV;
    _float2                             m_vNoiseCurrentUVForce;

    _float4                             m_vCurrentGlowColor;
    _float4                             m_vCurrentGlowColorForce;

    string                              m_szEffectName;
    EFFECTPARTICLE_DESC                 m_tEffectParticleDesc;
    vector<PARTICLE_DESC>               m_tParticleDescs;
    vector<PARTICLE_DESC>               m_tOriginalParticleDescs;

    _float                              m_fCurrentInitTime = 0.f;

    _bool                               m_bStopSprite = false;

    // For. Boner
    std::vector<std::string>            m_AllBoneNames;
    weak_ptr<CBoneNode>                 m_pBoneNode;
    weak_ptr<CModel>                    m_pParentModel;
    std::string                         m_strBoneName   = "";
    _int                                m_iCurrentBoneIndex = 0;

#ifdef _DEBUG
    // For. Thread
    _bool                   m_bResetTrigger = false;
    weak_ptr<CTransform>    m_pPreviewModelTransform;

#ifdef _JOJO_EFFECT_TOOL_
    // For. Tool
    _int m_iScaleType = 0;
    static const _int m_iScaleType_None;
    static const _int m_iScaleType_Square;
    static const _int m_iScaleType_Ratio;
#endif // _JOJO_EFFECT_TOOL_
#endif // _DEBUG
};

END
