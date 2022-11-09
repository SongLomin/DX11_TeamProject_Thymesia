#include "Model.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "GameObject.h"
#include "ModelData.h"
#include "Texture.h"
#include "Shader.h"
#include "BoneNode.h"
#include "Animation.h"

GAMECLASS_C(CModel)
CLONE_C(CModel, CComponent)

CModel::CModel(const CModel& rhs)
{
	*this = rhs;
}

weak_ptr<MODEL_DATA> CModel::Get_ModelData() const
{
	return m_pModelData;
}

_int CModel::Get_IndexFromAnimName(const _char* In_szAnimName)
{

	for (_int i = 0; i < m_iNumAnimations; ++i)
	{
		if (strcmp(m_Animations[i].lock()->Get_Name(), In_szAnimName) == 0)
		{
			return i;
		}
	}

	DEBUG_ASSERT;

	return -1;
}

void CModel::Set_CurrentAnimation(_uint iAnimIndex, _uint iStartKeyIndex, _float fBlendTime)
{
	if (iAnimIndex >= m_Animations.size())
		return;

	Reset_DeltaBonePositions();
	m_iCurrentAnimationIndex = iAnimIndex;
	m_Animations[m_iCurrentAnimationIndex].lock()->Reset_Animaition();
	m_Animations[m_iCurrentAnimationIndex].lock()->Set_StartAnimationKey(iStartKeyIndex);

	m_fMaxBlendTime = fBlendTime;
	m_fCurrentBlendTime = 0.f;

	m_isBlend = fBlendTime > 0.f;

	
	for (size_t i = 0; i < m_pBoneNodes.size(); i++)
	{
		m_pBoneNodes[i].lock()->Bake_PreKeyFrame();
	}
	
	
}

void CModel::Set_AnimationSpeed(_float fAnimationSpeed)
{
	for (size_t i = 0; i < m_Animations.size(); i++)
	{
		m_Animations[i].lock()->Set_AnimationSpeed(fAnimationSpeed);
	}

}

void CModel::Set_CurrentAnimationKey(_uint iKeyIndex)
{
	m_Animations[m_iCurrentAnimationIndex].lock()->Set_StartAnimationKey(iKeyIndex);
}

_vector CModel::Get_DeltaBonePosition(const char* In_szBoneName)
{
	if (m_isBlend)
	{
		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	_vector vCurrentBonePosition;
	_vector vPreBonePosition;

	weak_ptr<CBoneNode> CurrentBoneNode = Find_BoneNode(In_szBoneName);

	if (!CurrentBoneNode.lock().get())
		assert(false);

	string szBoneNameToString = In_szBoneName;
	size_t HashKey = hash<string>()(szBoneNameToString);

	unordered_map<size_t, _float4>::iterator iter = m_DeltaBonePositions.find(HashKey);

	if (m_DeltaBonePositions.end() == iter)
	{
		_float4 CurrentPosition;

		XMStoreFloat4(&CurrentPosition, CurrentBoneNode.lock()->Get_TransformationMatrix().r[3]);
		m_DeltaBonePositions.emplace(HashKey, CurrentPosition);

		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	vPreBonePosition = XMLoadFloat4(&(*iter).second);
	vCurrentBonePosition = CurrentBoneNode.lock()->Get_TransformationMatrix().r[3];
	vCurrentBonePosition -= vPreBonePosition;

	XMStoreFloat4(&(*iter).second, CurrentBoneNode.lock()->Get_TransformationMatrix().r[3]);

	vCurrentBonePosition.m128_f32[1] = 0.f;
	//vCurrentBonePosition.m128_f32[0] = 0.f;

	vCurrentBonePosition = XMVector3TransformCoord(vCurrentBonePosition, XMLoadFloat4x4(&m_pModelData->TransformMatrix));

	return vCurrentBonePosition;
}

_uint CModel::Get_CurrentAnimationKeyIndex() const
{
	return m_Animations[m_iCurrentAnimationIndex].lock()->Get_CurrentChannelKeyIndex();
}

_uint CModel::Get_CurrentAnimationIndex() const
{
	return m_iCurrentAnimationIndex;
}

vector<string> CModel::Get_AllAnimationKeys() const
{
	vector<string> AllAnimationKeys;

	AllAnimationKeys.reserve(m_Animations.size());

	for (_size_t i = 0; i < m_Animations.size(); ++i)
	{
		AllAnimationKeys.emplace_back(m_Animations[i].lock()->Get_Name());
	}

	return AllAnimationKeys;
}

vector<string> CModel::Get_AllBoneNames() const
{
	vector<string> AllBoneNames;

	AllBoneNames.reserve(m_pBoneNodes.size());

	for (_size_t i = 0; i < m_pBoneNodes.size(); ++i)
	{
		AllBoneNames.emplace_back(m_pBoneNodes[i].lock()->Get_Name());
	}

	return AllBoneNames;
}

_bool CModel::Is_CurrentAnimationKeyInRange(const _uint& iA, const _uint& iB) const
{
	_uint iCurrnetIndex = Get_CurrentAnimationKeyIndex();
	return iA <= iCurrnetIndex && iCurrnetIndex <= iB;
}

_float4x4 CModel::Get_TransformationMatrix() const
{
	return m_pModelData->TransformMatrix;
}

weak_ptr<CAnimation> CModel::Get_AnimationFromIndex(const _uint& In_iIndex) const
{
	if (In_iIndex >= m_Animations.size())
	{
		DEBUG_ASSERT;
	}

	return m_Animations[In_iIndex];
}

HRESULT CModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	USE_START(CModel);

	//GAMEINSTANCE->Load_Textures(TEXT("S01404_09_B_C"), TEXT("../Bin/Resources/Textures/Texture2D/S01404_09_B_C.png"));

	return S_OK;
}

void CModel::Start()
{
	USE_RENDER(CModel);
}

HRESULT CModel::Play_Animation(_float fTimeDelta)
{
	if (!Get_Enable())
		return E_FAIL;

	if (m_iCurrentAnimationIndex >= m_iNumAnimations)
		return E_FAIL;

	if (m_isBlend)
	{
		if (m_fMaxBlendTime < DBL_EPSILON || m_fCurrentBlendTime >= m_fMaxBlendTime)
		{
			m_Animations[m_iCurrentAnimationIndex].lock()->Update_TransformationMatrices(fTimeDelta);
			m_isBlend = false;
		}

		else
		{
			_float fRatio = m_fCurrentBlendTime / m_fMaxBlendTime;

			m_Animations[m_iCurrentAnimationIndex].lock()->Blend_Animation(m_fMaxBlendTime, fRatio);

			m_fCurrentBlendTime += fTimeDelta;
		}
	}
	else
	{
		m_Animations[m_iCurrentAnimationIndex].lock()->Update_TransformationMatrices(fTimeDelta);
	}

	for (auto& pBoneNode : m_pBoneNodes)
		pBoneNode.lock()->Update_CombinedTransformationMatrix();

	return S_OK;
}

HRESULT CModel::Render_Mesh(_uint iMeshContainerIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	m_MeshContainers[iMeshContainerIndex].lock()->Render();

	return S_OK;
}

HRESULT CModel::Render_AnimModel(_uint iMeshContainerIndex, weak_ptr<CShader> pShader, _uint iPassIndex, const char* pConstantBoneName)
{
	if (!Get_Enable())
		return E_FAIL;

	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	/* 그리고자하는 메시컨테이너에 영향을 주는 뼈들의 행렬을 담아준다. */
	_float4x4			BoneMatrices[256];
	//_float4x4*			BoneMatrices = new _float4x4[256];

	m_MeshContainers[iMeshContainerIndex].lock()->SetUp_BoneMatices(BoneMatrices, XMLoadFloat4x4(&m_pModelData->TransformMatrix));

	if (0 != m_iNumAnimations)
		pShader.lock()->Set_RawValue(pConstantBoneName, BoneMatrices, sizeof(_float4x4) * 256);

	pShader.lock()->Begin(iPassIndex);

	m_MeshContainers[iMeshContainerIndex].lock()->Render();

	//delete[](BoneMatrices);


	return S_OK;
}

void CModel::Init_Model(const char* sModelKey, const string& szTexturePath, _uint iTimeScaleLayer)
{
	Init_Model_Internal(sModelKey, szTexturePath, iTimeScaleLayer);
	
	//future<void> futureInitModel = async(launch::async, &CModel::Init_Model_Internal, this, sModelKey, szTexturePath);

	/*char			szDir[MAX_PATH] = "";
	strcpy_s(szDir, "../Bin/Resources/Meshes/");
	strcat_s(szDir, sModelKey);
	strcat_s(szDir, "/");
	Create_Materials(szDir);*/
}


void CModel::Init_Model_Internal(const char* sModelKey, const string& szTexturePath, _uint iTimeScaleLayer)
{
	//재사용할거라서 Reset은 필요하지 않음.
	//Reset_Model();

	Reset_Model();

	m_pModelData = GAMEINSTANCE->Get_ModelFromKey(sModelKey);

	if (!m_pModelData.get())
		assert(false);

	m_szModelKey = sModelKey;

	if (szTexturePath.empty())
	{
		char szDir[MAX_PATH];
		_splitpath_s(m_pModelData->szModelFilePath.c_str(), nullptr, 0, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
		Create_Materials(szDir);
	}
	else
	{
		Create_Materials(szTexturePath.c_str());
	}

	Create_BoneNodes(m_pModelData.get()->RootNode, weak_ptr<CBoneNode>(), 0);
	sort(m_pBoneNodes.begin(), m_pBoneNodes.end(), [](weak_ptr<CBoneNode> pSour, weak_ptr<CBoneNode> pDest)
		{
			return pSour.lock()->Get_Depth() < pDest.lock()->Get_Depth();
		});
	Create_MeshContainers();
	Create_Animations(iTimeScaleLayer);

}

void CModel::Reset_Model()
{
	for (auto& elem : m_MeshContainers)
	{
		m_pOwner.lock()->Remove_Component(elem);
	}

	for (auto& elem : m_Animations)
	{
		m_pOwner.lock()->Remove_Component(elem);
	}

	for (auto& elem : m_pBoneNodes)
	{
		m_pOwner.lock()->Remove_Component(elem);
	}

	for (auto& elem : m_Materials)
	{
		for (_uint i = 0; i < (_uint)AI_TEXTURE_TYPE_MAX; ++i)
		{
			if (elem.pTextures[i].lock())
			{
				m_pOwner.lock()->Remove_Component(elem.pTextures[i].lock());
			}
		}
	}

	m_pModelData.reset();
	m_iNumMeshContainers = 0;
	m_iNumMaterials = 0;
	m_MeshContainers.clear();
	m_Materials.clear();
	m_Animations.clear();
	m_pBoneNodes.clear();

}

void CModel::Reset_DeltaBonePositions()
{
	m_DeltaBonePositions.clear();
}

void CModel::Set_RootNode(const string& pBoneName, const _bool& In_bRoot)
{
	weak_ptr<CBoneNode> pBoneNode = Find_BoneNode(pBoneName);

	//해당하는 이름의 노드가 없음.
	if (!pBoneNode.lock().get())
		assert(false);

	pBoneNode.lock()->Set_bRootNode(In_bRoot);
}

void CModel::Write_Json(json& Out_Json)
{
	Out_Json.emplace("Model", m_szModelKey);
}

void CModel::Load_FromJson(const json& In_Json)
{
	m_szModelKey = In_Json["Model"];
	if (!m_szModelKey.empty())
	{
		Init_Model(m_szModelKey.c_str());
	}
}



HRESULT CModel::Bind_SRV(weak_ptr<CShader> pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		assert(false);

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex].lock()->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		assert(false);

	if (!m_Materials[iMaterialIndex].pTextures[eType].lock().get())
	{
		if (aiTextureType::aiTextureType_DIFFUSE == eType)
		{
			vector<ComPtr<ID3D11ShaderResourceView>> NullTexture = GAMEINSTANCE->Get_TexturesFromKey("NullTexture");

			 pShader.lock()->Set_ShaderResourceView(pConstantName, NullTexture.front());
		}

		
		return S_OK;
	}

	/*else
	{
		if (aiTextureType::aiTextureType_DIFFUSE == eType)
		{
			pShader.lock()->Set_ShaderResourceView(pConstantName, GAMEINSTANCE->Get_TexturesFromKey(TEXT("NullTexture")).front());
		}

		return E_FAIL;
	}*/

	return m_Materials[iMaterialIndex].pTextures[eType].lock()->Set_ShaderResourceView(pShader, pConstantName);
}

weak_ptr<CBoneNode> CModel::Find_BoneNode(const string& pBoneName)
{
	auto	iter = find_if(m_pBoneNodes.begin(), m_pBoneNodes.end(), [&](weak_ptr<CBoneNode> pNode)
		{
			return !strcmp(pBoneName.c_str(), pNode.lock()->Get_Name());
		});

	if (iter == m_pBoneNodes.end())
		return weak_ptr<CBoneNode>();

	return *iter;
}

void CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pModelData->iNumMeshs;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		weak_ptr<CMeshContainer> pMeshContainer = m_pOwner.lock()->Add_Component<CMeshContainer>();
		pMeshContainer.lock()->Init_Mesh(m_pModelData->Mesh_Datas[i], Cast<CModel>(m_this));
		m_MeshContainers.push_back(pMeshContainer);
	}
}

void CModel::Create_Materials(const char* pModelFilePath)
{

	m_iNumMaterials = m_pModelData->iNumMaterials;

#ifdef _DEBUG
	cout << m_szModelKey << endl;
	cout << "========================================" << endl;
	cout << "NumMaterials: " << m_iNumMaterials << endl;
#endif // _DEBUG


	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < (_uint)AI_TEXTURE_TYPE_MAX; ++j)
		{
			char			szFullPath[MAX_PATH] = "";

			string		strPath;

			//if (FAILED(AISCENE->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
			//	continue;

			strPath = m_pModelData->Material_Datas[i]->szTextureName[j];


			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
			//_tchar		szTextureKey[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szTextureFilePath, MAX_PATH);
			//MultiByteToWideChar(CP_ACP, 0, szFileName, (_int)strlen(szFileName), szTextureKey, MAX_PATH);

#ifdef _DEBUG
			//cout << "Load_Texture: " << szFullPath << endl;
#endif // _DEBUG

			GAMEINSTANCE->Load_Textures(szFileName, szTextureFilePath, MEMORY_TYPE::MEMORY_STATIC);
			Material.pTextures[j] = m_pOwner.lock()->Add_Component<CTexture>();
			Material.pTextures[j].lock().get()->Use_Texture(szFileName);

		}

		m_Materials.push_back(Material);
	}
}

void CModel::Create_BoneNodes(shared_ptr<NODE_DATA> pNodeData, weak_ptr<CBoneNode> pParent, _uint iDepth)
{
	weak_ptr<CBoneNode> pBoneNode = m_pOwner.lock()->Add_Component<CBoneNode>();
	pBoneNode.lock()->Init_BoneNode(pNodeData, pParent, iDepth);
	m_pBoneNodes.push_back(pBoneNode);

	for (_uint i = 0; i < pNodeData->iNumChildren; ++i)
		Create_BoneNodes(pNodeData->pChildren[i], pBoneNode, iDepth + 1);
}

void CModel::Create_Animations(_uint iTimeScaleLayer)
{
	m_iNumAnimations = m_pModelData->iNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		weak_ptr<CAnimation> pAnimation = m_pOwner.lock()->Add_Component<CAnimation>();
		pAnimation.lock()->Init_Animation(m_pModelData->Animation_Datas[i], Cast<CModel>(m_this), iTimeScaleLayer);

		m_Animations.push_back(pAnimation);
	}

}

void CModel::OnDestroy()
{
	UNUSE_START(CModel);
	UNUSE_RENDER(CModel);
}

void CModel::Free()
{

}
