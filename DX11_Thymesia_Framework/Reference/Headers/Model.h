#pragma once
#include "Component.h"
BEGIN(Engine)

struct MODEL_DATA;
struct NODE_DATA;
class CMeshContainer;
class CShader;
class CTexture;
class CAnimation;
class CBoneNode;

class ENGINE_DLL CModel : public CComponent
{
	GAMECLASS_H(CModel);
	CLONE_H(CModel, CComponent);

public:
	typedef struct tagModelMaterial
	{
		weak_ptr<CTexture> pTextures[AI_TEXTURE_TYPE_MAX];
	}MODEL_MATERIAL;


public:
	CModel(const CModel& rhs);

public:
	_uint Get_NumMeshContainers() const {
		return m_iNumMeshContainers;
	}

	const _char* Get_ModelKey() const
	{
		return m_szModelKey.c_str();
	}

	weak_ptr<MODEL_DATA> Get_ModelData() const;

	_int Get_IndexFromAnimName(const _char* In_szAnimName);

	void Set_CurrentAnimation(_uint iAnimIndex, _uint iStartKeyIndex = 0, _float fBlendTime = 0.1f);
	void Set_AnimationSpeed(_float fAnimationSpeed);
	void Set_CurrentAnimationKey(_uint iKeyIndex);
	

	_vector Get_DeltaBonePosition(const char* In_szBoneName);
	_vector Get_DeltaBonePitchYawRoll(const char* In_szBoneName);

	_uint Get_CurrentAnimationKeyIndex() const;
	_uint Get_CurrentAnimationIndex() const;

	vector<string> Get_AllAnimationKeys() const;
	vector<string> Get_AllBoneNames() const;

	_bool Is_CurrentAnimationKeyInRange(const _uint& iA, const _uint& iB) const;

	weak_ptr<CAnimation>	Get_CurrentAnimation() const
	{
		return m_Animations[m_iCurrentAnimationIndex];
	}

	_float4x4 Get_TransformationMatrix() const;

	weak_ptr<CAnimation> Get_AnimationFromIndex(const _uint& In_iIndex) const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Render_Mesh(_uint iMeshContainerIndex);
	HRESULT Render_AnimModel(_uint iMeshContainerIndex, weak_ptr<CShader> pShader, _uint iPassIndex, const char* pConstantBoneName);
	virtual void OnDestroy() override;
	
public:
	void Init_Model(const char* sModelKey, const string& szTexturePath = "", _uint iTimeScaleLayer = 0);
	HRESULT Bind_SRV(weak_ptr<CShader> pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eActorType);
	weak_ptr<CBoneNode> Find_BoneNode(const string& pBoneName);
	void Reset_DeltaBonePositions();
	void Set_RootNode(const string& pBoneName, const _bool& In_bRoot = true);
	
public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	void Init_Model_Internal(const char* sModelKey, const string& szTexturePath = "", _uint iTimeScaleLayer = 0);
	void Reset_Model();
	

private:
	void Create_MeshContainers();
	void Create_Materials(const char* pModelFilePath);
	void Create_BoneNodes(shared_ptr<NODE_DATA> pNodeData, weak_ptr<CBoneNode> pParent, _uint iDepth);
	void Create_Animations(_uint iTimeScaleLayer);

protected:
	shared_ptr<MODEL_DATA>					m_pModelData;
	vector<weak_ptr<CMeshContainer>>		m_MeshContainers;
	_uint									m_iNumMeshContainers = 0;
	_uint									m_iNumMaterials = 0;

	vector<MODEL_MATERIAL>					m_Materials;

	string									m_szModelKey;

private:
	vector<weak_ptr<CBoneNode>>				m_pBoneNodes;
	vector<weak_ptr<CBoneNode>>				HIERARCHYNODES;

private:
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iNumAnimations;
	vector<weak_ptr<CAnimation>>			m_Animations;
	typedef vector<weak_ptr<CAnimation>>	ANIMATIONS;

	_bool									m_isBlend = false;
	_float									m_fCurrentBlendTime = 0.f;
	_float									m_fMaxBlendTime;

private:
	unordered_map<size_t, _float4>			m_DeltaBonePositions;
	unordered_map<size_t, _float3>			m_DeltaBoneRotations;

public:
	FDelegate<>								CallBack_AnimationEnd;

public:
	void Free();
};

END