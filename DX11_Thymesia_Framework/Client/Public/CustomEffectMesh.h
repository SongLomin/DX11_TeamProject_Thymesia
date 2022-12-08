#pragma once
#include "EffectMesh.h"

BEGIN(Engine)
class CTransform;
class CBoneNode;
END

BEGIN(Client)
class CSkill;
class CAttackArea;
class CCustomEffectMesh final :
    public CEffectMesh
{
    GAMECLASS_H(CCustomEffectMesh)
    SHALLOW_COPY(CCustomEffectMesh)
    CLONE_H(CCustomEffectMesh, CGameObject)

public: // Get & Set
	const _char* Get_EffectName() const;
	_bool Is_Finish() const;
	void Set_TimeScaleLayer(const _uint& In_iTimeScaleLayer) { m_iTimeScaleLayerIndex = In_iTimeScaleLayer; }
    void Reset_Effect(weak_ptr<CTransform> pParentTransform);
    virtual void OnEventMessage(_uint iArg) override;
    void OnChangeAnimationKey(const _uint& In_Key);
    void Init_EffectMesh(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey);
    void Sync_Animation();
    void Play(_float fTimeDelta);
    void Write_FromBinary(ostream& os);
    void Load_FromBinary(istream& is);
    void Write_EffectJson(json& Out_Json);
    void Load_EffectJson(const json& In_Json, const _uint& In_iTimeScaleLayer);

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    void Free();

    virtual void SetUp_ShaderResource() override;
    virtual void SetUp_ShaderResource_Dissolve();

    virtual void OnEnable(void* _Arg = nullptr) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

    void Play_Internal  (_float fFrameTime);
    void Update_Position(_float fFrameTime);
    void Update_Rotation(_float fFrameTime);
    void Update_Scale   (_float fFrameTime);
    void Update_Color   (_float fFrameTime);
    void Update_Diffuse (_float fFrameTime);
    void Updaste_Noise  (_float fFrameTime);
    void Update_Mask    (_float fFrameTime);
    void Update_Glow    (_float fFrameTime);

    // For. Easing
    void Apply_Easing
    (
        _float2& vTarget
        , EASING_TYPE eEasingType
        , _vector   vStartPoint
        , _vector   vTargetPoint
        , _float    fElapsedTime
        , _float    fTotalTime
    );


#ifdef _DEBUG
    void Clone_EffectMesh();
#ifdef _JOJO_EFFECT_TOOL_
    void Key_Input_ControlMesh(_float fTimeDelta);
    void Apply_ImGui_Controls_to_Mesh();
#endif // _JOJO_EFFECT_TOOL_
    void Tool_Control();
    void Tool_Spawn_Life_Time();
    void Tool_AnimationSync();
    void Tool_Boner();
    void Tool_Position();
    void Tool_Speed();
    void Tool_Rotation();
    void Tool_Scale();
    void Tool_Shaders();
    void Tool_Dissolve();
    void Tool_Colors();
    void Tool_Texture_Diffuse();
    void Tool_Texture_Noise();
    void Tool_Texture_Mask();
    void Tool_Collider();
#endif // _DEBUG

private:
    weak_ptr<CAttackArea> m_pAttackArea;
    weak_ptr<CTransform>  m_pParentTransformCom;

    _uint                 m_iTimeScaleLayerIndex;
    _bool                 m_bFinish;
    _float4x4             m_ParentMatrix;
    _float                m_fPreFrame;

    string                m_szEffectName;
    EFFECTMESH_DESC       m_tEffectMeshDesc;

    weak_ptr<CTexture>    m_pColorDiffuseTextureCom;
    weak_ptr<CTexture>    m_pMaskTextureCom;
    weak_ptr<CTexture>    m_pNoiseTextureCom;
    weak_ptr<CTexture>    m_pDissolveTextureCom;

    _uint m_iNumMeshContainers;

    _float                m_fCurrentLifeTime = 0.f;
    _float                m_fCurrentInitTime = 0.f;

    // For. Position
    _float3               m_vCurrentSpeed;
    _float3               m_vCurrentForce;

    // For. Rotation
    _float3               m_vCurrentRotation;
    _float3               m_vCurrentRotationForce;

    // For. Scale
    _float3               m_vCurrentScale;
    _float3               m_vCurrentScaleSpeed;
    _float3               m_vCurrentScaleForce;

    // For. Color
    _float4               m_vCurrentColor;
    _float4               m_vCurrentColorForce;


    // For. UV
    _float2               m_vDiffuseCurrentUV;
    _float2               m_vDiffuseCurrentUVForce;
    _float2               m_vNoiseCurrentUV;
    _float2               m_vNoiseCurrentUVForce;
    _float2               m_vMaskCurrentUV;
    _float2               m_vMaskCurrentUVForce;

    // For. Glow
    _float4               m_vCurrentGlowColor;
    _float4               m_vCurrentGlowForce;

    // For. Boner
    std::vector<std::string>            m_AllBoneNames;
    weak_ptr<CBoneNode>                 m_pBoneNode;
    weak_ptr<CModel>                    m_pParentModel;
    std::string                         m_strBoneName = "";
    _int                                m_iCurrentBoneIndex = 0;
};
END
