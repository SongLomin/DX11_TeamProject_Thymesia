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

	typedef struct tag_NvCloth_Mesh_desc
	{
		SELECTION_TYPE	eSimpleAttachType = SELECTION_TYPE::TYPE_END;
		_float			fSimpleAttachRatio = 0.f;
		_float			fSimpleInvMess = 1.f;

		// It is not used when using Simple Attach.
		vector<_float>	InvMesses;

		_float3			vGravity = _float3(0.f, 9.81f, 0.f);
		_float			fDragCoefficient = 0.f;
		_float			fLiftCoefficient = 0.f;
		_bool			m_bUseBoneMatrix = false;
	}NVCLOTH_MESH_DESC;

	typedef struct tag_NvCloth_Model_desc
	{
		map<_uint, tag_NvCloth_Mesh_desc> NvClothMeshDescs;
	}NVCLOTH_MODEL_DESC;


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
	

	_vector Get_DeltaBonePosition(const char* In_szBoneName, const _bool In_bUseOffset = false , _fmatrix In_OffsetMatrix = XMMatrixIdentity());
	_flag	Get_RootBoneFlag(const char* In_szBoneName);
	//_vector Get_DeltaBonePitchYawRoll(const char* In_szBoneName);

	_uint Get_CurrentAnimationKeyIndex() const;
	_uint Get_CurrentAnimationIndex() const;

	vector<string> Get_AllAnimationKeys() const;
	vector<string> Get_AllBoneNames() const;

	MESH_VTX_INFO Get_MeshVertexInfo();

	_bool Is_CurrentAnimationKeyInRange(const _uint& iA, const _uint& iB) const;

	weak_ptr<CAnimation>	Get_CurrentAnimation() const
	{
		return m_Animations[m_iCurrentAnimationIndex];
	}

	 _float4x4 Get_TransformationMatrix() const;

	weak_ptr<CAnimation> Get_AnimationFromIndex(const _uint& In_iIndex) const;

	weak_ptr<CMeshContainer> Get_MeshContainer(const _uint In_iIndex) const;

	_bool IsModelPicking(const RAY& In_Ray, _float& Out_fRange);

	weak_ptr<CBoneNode> Find_BoneNode(const string& pBoneName);
	void Set_RootNode(const string& pBoneName, const _byte RootNodeFlags);

private: /* CGameObject Based */
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

public: /* Init */
	void Init_Model(const char* sModelKey, const string& szTexturePath = "", _uint iTimeScaleLayer = 0, const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc = nullptr);
	void Init_Model(shared_ptr<MODEL_DATA> pModelData, const string& szTexturePath = "", _uint iTimeScaleLayer = 0, const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc = nullptr);

private:
	void Init_Model_Internal(shared_ptr<MODEL_DATA> pModelData, const string& szTexturePath = "", _uint iTimeScaleLayer = 0, const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc = nullptr);
	void Reset_Model();

private:
	void Create_MeshContainers(const NVCLOTH_MODEL_DESC* In_pNvClothMeshDesc = nullptr);
	void Create_Materials(const char* pModelFilePath);
	void Create_ORM_Material(MODEL_MATERIAL& Out_Material, const _uint In_iMaterialIndex, const _char* pModelFilePath);
	void Create_BoneNodes(shared_ptr<NODE_DATA> pNodeData, weak_ptr<CBoneNode> pParent, _uint iDepth);
	void Create_Animations(_uint iTimeScaleLayer);

public: /* Loopable functions */
	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Bind_SRV(weak_ptr<CShader> pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eActorType);
	HRESULT Render_Mesh(_uint iMeshContainerIndex, ID3D11DeviceContext* pDeviceContext);
	HRESULT Update_BoneMatrices();
	HRESULT Update_NvCloth(ID3D11DeviceContext* pDeferredContext, _fmatrix In_WorldMatrix);
	HRESULT Render_AnimModel(_uint iMeshContainerIndex, weak_ptr<CShader> pShader, _uint iPassIndex, const char* pConstantBoneName, ID3D11DeviceContext* pDeviceContext);
	
public:
	//void Set_NvClothMeshWithIndex(const _uint In_iIndex);

public: 
	void Add_ReverseAnimation(const _uint In_iAnimationIndex, _uint iTimeScaleLayer);
	void Reset_DeltaBonePositions();

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
	_float									m_fStartBlendTime;

private:
	unordered_map<size_t, _float4>			m_DeltaBonePositions;

public:
	FDelegate<_uint>						CallBack_AnimationEnd;

private:
	mutex m_JobMutex;

private:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	virtual void OnDestroy() override;
	void Free();
};

END