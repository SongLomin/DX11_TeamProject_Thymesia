#include "Model.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "GameObject.h"
#include "ModelData.h"
#include "Texture.h"
#include "Shader.h"
#include "BoneNode.h"
#include "Animation.h"
#include "SMath.h"
#include "Resource_Manager.h"

GAMECLASS_C(CModel)
CLONE_C(CModel, CComponent)

CModel::CModel(const CModel& rhs)
{
	this->m_pOwner = rhs.m_pOwner;
}

weak_ptr<MODEL_DATA> CModel::Get_ModelData() const
{
	return m_pModelData;
}

_int CModel::Get_IndexFromAnimName(const _char* In_szAnimName)
{

	for (_uint i(0); i < m_iNumAnimations; ++i)
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

	m_fStartBlendTime = m_Animations[m_iCurrentAnimationIndex].lock()->Get_TimeAcc();

	m_fMaxBlendTime = fBlendTime + m_fStartBlendTime;
	m_fCurrentBlendTime = m_fStartBlendTime;

	m_isBlend = true;


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

_vector CModel::Get_DeltaBonePosition(const char* In_szBoneName, const _bool In_bUseOffset, _fmatrix In_OffsetMatrix)
{
	std::unique_lock<std::mutex> lock(m_JobMutex);

	if (m_isBlend)
	{
		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	_vector vCurrentBonePosition;
	_vector vPreBonePosition;

	weak_ptr<CBoneNode> CurrentBoneNode = Find_BoneNode(In_szBoneName);

	if (!CurrentBoneNode.lock())
		assert(false);

	string szBoneNameToString = In_szBoneName;
	size_t HashKey = hash<string>()(szBoneNameToString);

	if (m_DeltaBonePositions.empty())
	{
		_float4 CurrentPosition;

		XMStoreFloat4(&CurrentPosition, CurrentBoneNode.lock()->Get_TransformationMatrix().r[3]);
		m_DeltaBonePositions.emplace(HashKey, CurrentPosition);

		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	unordered_map<size_t, _float4>::iterator iter;
	iter = m_DeltaBonePositions.find(HashKey);


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

	if (In_bUseOffset)
	{
		vCurrentBonePosition = XMVector3TransformCoord(vCurrentBonePosition, In_OffsetMatrix);
	}
	else
	{
		vCurrentBonePosition = XMVector3TransformCoord(vCurrentBonePosition, XMLoadFloat4x4(&m_pModelData->TransformMatrix));
	}

	_byte RootNodeFlags = CurrentBoneNode.lock()->Get_RootNodeFlags();

	if (!(RootNodeFlags & (_byte)ROOTNODE_FLAG::X))
	{
		vCurrentBonePosition.m128_f32[0] = 0.f;
	}

	if (!(RootNodeFlags & (_byte)ROOTNODE_FLAG::Y))
	{
		vCurrentBonePosition.m128_f32[1] = 0.f;
	}

	if (!(RootNodeFlags & (_byte)ROOTNODE_FLAG::Z))
	{
		vCurrentBonePosition.m128_f32[2] = 0.f;
	}

	return vCurrentBonePosition;
}

_flag CModel::Get_RootBoneFlag(const char* In_szBoneName)
{
	weak_ptr<CBoneNode> CurrentBoneNode = Find_BoneNode(In_szBoneName);

	return CurrentBoneNode.lock()->Get_RootNodeFlags();
}

//_vector CModel::Get_DeltaBonePitchYawRoll(const char* In_szBoneName)
//{
//	if (m_isBlend)
//	{
//		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
//	}
//
//	_float3 vCurrentBonePitchYawRollFromFloat3;
//	_vector vCurrentBonePitchYawRoll;
//	_vector vPreBonePitchYawRoll;
//
//	weak_ptr<CBoneNode> CurrentBoneNode = Find_BoneNode(In_szBoneName);
//
//	if (!CurrentBoneNode.lock().get())
//		assert(false);
//
//	string szBoneNameToString = In_szBoneName;
//	size_t HashKey = hash<string>()(szBoneNameToString);
//
//	unordered_map<size_t, _float3>::iterator iter = m_DeltaBoneRotations.find(HashKey);
//
//	if (m_DeltaBoneRotations.end() == iter)
//	{
//
//		_float3 StartPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(CurrentBoneNode.lock()->Get_TransformationMatrix()));
//
//		m_DeltaBoneRotations.emplace(HashKey, StartPitchYawRoll);
//
//		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
//	}
//
//	vPreBonePitchYawRoll = XMLoadFloat3(&(*iter).second);
//	vCurrentBonePitchYawRollFromFloat3 = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(CurrentBoneNode.lock()->Get_TransformationMatrix()));
//	vCurrentBonePitchYawRoll = XMLoadFloat3(&vCurrentBonePitchYawRollFromFloat3);
//
//	vCurrentBonePitchYawRoll -= vPreBonePitchYawRoll;
//
//	(*iter).second = vCurrentBonePitchYawRollFromFloat3;
//
//	// XZ축 회전 소거
//	vCurrentBonePitchYawRoll.m128_f32[0] = 0.f;
//	vCurrentBonePitchYawRoll.m128_f32[2] = 0.f;
//
//	//vCurrentBonePosition = XMVector3TransformCoord(vCurrentBonePosition, XMLoadFloat4x4(&m_pModelData->TransformMatrix));
//
//	return vCurrentBonePitchYawRoll;
//}

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

MESH_VTX_INFO CModel::Get_MeshVertexInfo()
{
	return m_pModelData.get()->VertexInfo;
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

weak_ptr<CMeshContainer> CModel::Get_MeshContainer(const _uint In_iIndex) const
{
	return m_MeshContainers[In_iIndex];
}

_bool CModel::IsModelPicking(const RAY& In_Ray, _float& Out_fRange)
{
	if (m_pModelData)
		return false;

	GRAPHICDESC tGraphicDesc = GAMEINSTANCE->Get_GraphicDesc();

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(tGraphicDesc.iWinCX, tGraphicDesc.iWinCY);

	MESH_VTX_INFO Info = m_pModelData->VertexInfo;
	_float fDist;

	if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, m_pOwner.lock()->Get_Transform()->Get_WorldMatrix(), &fDist))
	{
		if (Out_fRange > fDist)
		{
			Out_fRange = fDist;
			return true;
		}
	}

	return false;
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
			m_Animations[m_iCurrentAnimationIndex].lock()->Set_TimeAcc(m_fCurrentBlendTime);
		}

		else
		{
			_float fRatio = (m_fCurrentBlendTime - m_fStartBlendTime) / (m_fMaxBlendTime - m_fStartBlendTime);

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

HRESULT CModel::Render_Mesh(_uint iMeshContainerIndex, ID3D11DeviceContext* pDeviceContext)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	m_MeshContainers[iMeshContainerIndex].lock()->Render(pDeviceContext);

	return S_OK;
}

HRESULT CModel::Update_BoneMatrices()
{
	for (auto& elem : m_MeshContainers)
	{
		elem.lock()->SetUp_BoneMatices(XMLoadFloat4x4(&m_pModelData->TransformMatrix));
	}

	return S_OK;
}

HRESULT CModel::Update_NvCloth(ID3D11DeviceContext* pDeferredContext, _fmatrix In_WorldMatrix)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.lock()->Update_NvClothVertices(pDeferredContext, In_WorldMatrix);
	}

	return S_OK;
}

HRESULT CModel::Render_AnimModel(_uint iMeshContainerIndex, weak_ptr<CShader> pShader, _uint iPassIndex, const char* pConstantBoneName, ID3D11DeviceContext* pDeviceContext)
{
	if (!Get_Enable())
		return E_FAIL;

	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	/* 그리고자하는 메시컨테이너에 영향을 주는 뼈들의 행렬을 담아준다. */
	//_float4x4			BoneMatrices[256];
	//_float4x4*			BoneMatrices = new _float4x4[256];

	if (0 != m_iNumAnimations)
	{
		if (FAILED(m_MeshContainers[iMeshContainerIndex].lock()->Bind_BoneMatices(pShader, pConstantBoneName)))
		{
#ifdef _DEBUG
			cout << "Failed to Bind bonematrix." << endl;
#endif // _DEBUG

			return E_FAIL;
		}
	}

	pShader.lock()->Begin(iPassIndex, pDeviceContext);

	m_MeshContainers[iMeshContainerIndex].lock()->Render(pDeviceContext);

	//delete[](BoneMatrices);


	return S_OK;
}

//void CModel::Set_NvClothMeshWithIndex(const _uint In_iIndex)
//{
//	m_MeshContainers[In_iIndex].lock()->Set_NvCloth();
//}

void CModel::Init_Model(const char* sModelKey, const string& szTexturePath, _uint iTimeScaleLayer, const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc)
{
	Reset_Model();

	m_pModelData = GAMEINSTANCE->Get_ModelFromKey(sModelKey);

	if (!m_pModelData)
		assert(false);

	m_szModelKey = sModelKey;

	Init_Model_Internal(m_pModelData, szTexturePath, iTimeScaleLayer, In_pNvClothModelDesc);

	//future<void> futureInitModel = async(launch::async, &CModel::Init_Model_Internal, this, sModelKey, szTexturePath);

	/*char			szDir[MAX_PATH] = "";
	strcpy_s(szDir, "../Bin/Resources/Meshes/");
	strcat_s(szDir, sModelKey);
	strcat_s(szDir, "/");
	Create_Materials(szDir);*/
}

void CModel::Init_Model(shared_ptr<MODEL_DATA> pModelData, const string& szTexturePath, _uint iTimeScaleLayer, const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc)
{
	Reset_Model();

	m_pModelData = pModelData;

	if (!m_pModelData)
		assert(false);

	m_szModelKey = pModelData->szModelFileName;

	Init_Model_Internal(m_pModelData, szTexturePath, iTimeScaleLayer, In_pNvClothModelDesc);
}


void CModel::Init_Model_Internal(shared_ptr<MODEL_DATA> pModelData, const string& szTexturePath, _uint iTimeScaleLayer, const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc)
{
	
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
	std::sort(m_pBoneNodes.begin(), m_pBoneNodes.end(), [](weak_ptr<CBoneNode> pSour, weak_ptr<CBoneNode> pDest)
		{
			return pSour.lock()->Get_Depth() < pDest.lock()->Get_Depth();
		});
	Create_MeshContainers(In_pNvClothModelDesc);
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
	m_iNumAnimations = 0;
	m_iNumMaterials = 0;
	m_MeshContainers.clear();
	m_Materials.clear();
	m_Animations.clear();
	m_pBoneNodes.clear();
	m_iCurrentAnimationIndex = 0;

}

void CModel::Reset_DeltaBonePositions()
{
	std::unique_lock<std::mutex> lock(m_JobMutex);

	m_DeltaBonePositions.clear();

	lock.unlock();
}

void CModel::Set_RootNode(const string& pBoneName, const _byte RootNodeFlags)
{
	weak_ptr<CBoneNode> pBoneNode = Find_BoneNode(pBoneName);

	//해당하는 이름의 노드가 없음.
	if (!pBoneNode.lock().get())
		assert(false);

	cout << "Set_RootNode" << (_int)RootNodeFlags << endl;

	pBoneNode.lock()->Set_RootNodeFlags(RootNodeFlags);
}

void CModel::Write_Json(json& Out_Json)
{
	Out_Json.emplace("Model", m_szModelKey);
}

void CModel::Load_FromJson(const json& In_Json)
{
	if (In_Json.end() == In_Json.find("Model"))
		return;

	m_szModelKey = In_Json["Model"];
	if (!m_szModelKey.empty())
	{
		Init_Model(m_szModelKey.c_str());
	}
}



HRESULT CModel::Bind_SRV(weak_ptr<CShader> pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eActorType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		assert(false);

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex].lock()->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		assert(false);

	function<void()> Bind_NullTexture = [&pShader, &pConstantName]()
	{
		vector<ComPtr<ID3D11ShaderResourceView>> NullTexture = GAMEINSTANCE->Get_TexturesFromKey("NullTexture");

		pShader.lock()->Set_ShaderResourceView(pConstantName, NullTexture.front());
	};

	if (!m_Materials[iMaterialIndex].pTextures[eActorType].lock().get())
	{
		if (aiTextureType::aiTextureType_DIFFUSE == eActorType)
		{
			Bind_NullTexture();
		}


		return E_FAIL;
	}

	/*else
	{
		if (aiTextureType::aiTextureType_DIFFUSE == eType)
		{
			pShader.lock()->Set_ShaderResourceView(pConstantName, GAMEINSTANCE->Get_TexturesFromKey(TEXT("NullTexture")).front());
		}

		return E_FAIL;
	}*/

	if (FAILED(m_Materials[iMaterialIndex].pTextures[eActorType].lock()->Set_ShaderResourceView(pShader, pConstantName)) &&
		aiTextureType::aiTextureType_DIFFUSE == eActorType)
	{
		Bind_NullTexture();

		return E_FAIL;
	}

	return S_OK;
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

void CModel::Create_MeshContainers(const NVCLOTH_MODEL_DESC* In_pNvClothModelDesc)
{
	m_iNumMeshContainers = m_pModelData->iNumMeshs;

	

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		const NVCLOTH_MESH_DESC* pMeshDesc = nullptr;

		if (In_pNvClothModelDesc)
		{
			auto iter = In_pNvClothModelDesc->NvClothMeshDescs.find(i);

			if (In_pNvClothModelDesc->NvClothMeshDescs.end() != iter)
			{
				pMeshDesc = &(*iter).second;
			}
		}

		weak_ptr<CMeshContainer> pMeshContainer = m_pOwner.lock()->Add_Component<CMeshContainer>();
		pMeshContainer.lock()->Init_Mesh(m_pModelData->Mesh_Datas[i], Cast<CModel>(m_this), pMeshDesc);
		m_MeshContainers.push_back(pMeshContainer);
	}
}

void CModel::Create_Materials(const char* pModelFilePath)
{

	m_iNumMaterials = m_pModelData->iNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < (_uint)AI_TEXTURE_TYPE_MAX; ++j)
		{
			//char			szFullddsPath[MAX_PATH] = "";
			//char			szFullpngPath[MAX_PATH] = "";
			string			szFullPath;

			string		strPath;

			//if (FAILED(AISCENE->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
			//	continue;

			strPath = m_pModelData->Material_Datas[i]->szTextureName[j];

			if (strPath.empty()) //스페큘러 인덱스 처리. 함수 따로 파서 ㅇㅇ 모델이 논애님이고
			{
				if (j == (_uint)aiTextureType_SPECULAR)
				{
					Create_ORM_Material(Material, i, pModelFilePath);
				}

				continue;
			}


			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			szFullPath = pModelFilePath;
			szFullPath += szFileName;


			tstring		szTextureFileddsPath = filesystem::path(szFullPath + ".dds").wstring();
			tstring		szTextureFilepngPath = filesystem::path(szFullPath + ".png").wstring();

#ifdef _DEBUG
			//cout << "Load_Texture: " << szFullPath << endl;
#endif // _DEBUG

			HRESULT hr = GET_SINGLE(CResource_Manager)->Load_Textures(szFileName, szTextureFileddsPath.c_str(), MEMORY_TYPE::MEMORY_STATIC);

			if (FAILED(hr))
			{
				hr = GET_SINGLE(CResource_Manager)->Load_Textures(szFileName, szTextureFilepngPath.c_str(), MEMORY_TYPE::MEMORY_STATIC);
			}

			if (SUCCEEDED(hr))
			{
				Material.pTextures[j] = m_pOwner.lock()->Add_Component<CTexture>();
				Material.pTextures[j].lock().get()->Use_Texture(szFileName);
			}

		}

		m_Materials.push_back(Material);
	}
}

void CModel::Create_ORM_Material(MODEL_MATERIAL& Out_Material, const _uint In_iMaterialIndex, const _char* pModelFilePath)
{
	string			szFullPath;

	string		strPath;

	strPath = m_pModelData->Material_Datas[In_iMaterialIndex]->szTextureName[(_uint)aiTextureType_DIFFUSE];


	char			szFileName[MAX_PATH] = "";
	char			szExt[MAX_PATH] = "";
	string			szFileNameToString;

	_splitpath_s(strPath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
	szFileNameToString = szFileName;

	/*filesystem::path strPathToPathType = filesystem::path(strPath);
	string			szFileName = strPathToPathType.filename().string();*/
	szFileNameToString = szFileNameToString.substr(0, szFileNameToString.size() - 1);
	szFileNameToString += "ORM";

	szFullPath = pModelFilePath;
	szFullPath += szFileNameToString;

	tstring		szTextureFileddsPath = filesystem::path(szFullPath + ".dds").wstring();
	tstring		szTextureFilepngPath = filesystem::path(szFullPath + ".png").wstring();


	//MultiByteToWideChar(CP_ACP, 0, szFullddsPath, (_int)strlen(szFullddsPath), szTextureFileddsPath, MAX_PATH);
	//MultiByteToWideChar(CP_ACP, 0, szFullpngPath, (_int)strlen(szFullpngPath), szTextureFilepngPath, MAX_PATH);

#ifdef _DEBUG
			//cout << "Load_Texture: " << szFullPath << endl;
#endif // _DEBUG

	HRESULT hr = GET_SINGLE(CResource_Manager)->Load_Textures(szFileNameToString.c_str(), szTextureFileddsPath.c_str(), MEMORY_TYPE::MEMORY_STATIC);

	if (FAILED(hr))
	{
		hr = GET_SINGLE(CResource_Manager)->Load_Textures(szFileNameToString.c_str(), szTextureFilepngPath.c_str(), MEMORY_TYPE::MEMORY_STATIC);
	}

	if (SUCCEEDED(hr))
	{
		Out_Material.pTextures[(_uint)aiTextureType_SPECULAR] = m_pOwner.lock()->Add_Component<CTexture>();
		Out_Material.pTextures[(_uint)aiTextureType_SPECULAR].lock().get()->Use_Texture(szFileNameToString.c_str());
	}


}

void CModel::Create_BoneNodes(shared_ptr<NODE_DATA> pNodeData, weak_ptr<CBoneNode> pParent, _uint iDepth)
{
	weak_ptr<CBoneNode> pBoneNode = m_pOwner.lock()->Add_Component<CBoneNode>();
	pBoneNode.lock()->Init_BoneNode(pNodeData, pParent, iDepth, m_pModelData->TransformMatrix);
	m_pBoneNodes.emplace_back(pBoneNode);

	for (_uint i = 0; i < pNodeData->iNumChildren; ++i)
		Create_BoneNodes(pNodeData->pChildren[i], pBoneNode, iDepth + 1);
}

void CModel::Create_Animations(_uint iTimeScaleLayer)
{
	m_iNumAnimations = m_pModelData->iNumAnimations;

	weak_ptr<CModel> ThisToModel = Weak_StaticCast<CModel>(m_this);

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		weak_ptr<CAnimation> pAnimation = m_pOwner.lock()->Add_Component<CAnimation>();
		pAnimation.lock()->Init_Animation(m_pModelData->Animation_Datas[i], ThisToModel, iTimeScaleLayer);

		m_Animations.emplace_back(pAnimation);
	}

}

void CModel::Add_ReverseAnimation(const _uint In_iAnimationIndex, _uint iTimeScaleLayer)
{
	weak_ptr<CModel> ThisToModel = Weak_StaticCast<CModel>(m_this);

	shared_ptr<ANIMATION_DATA> pReverseAnimationData = make_shared<ANIMATION_DATA>();
	m_pModelData->Animation_Datas[In_iAnimationIndex]->Bake_ReverseAnimation(pReverseAnimationData);
	m_pModelData->Animation_Datas.emplace_back(pReverseAnimationData);

	weak_ptr<CAnimation> pAnimation = m_pOwner.lock()->Add_Component<CAnimation>();
	pAnimation.lock()->Init_Animation(pReverseAnimationData, ThisToModel, iTimeScaleLayer);
	m_Animations.emplace_back(pAnimation);

	m_iNumAnimations = ++m_pModelData->iNumAnimations;
}

void CModel::OnDestroy()
{
	UNUSE_START(CModel);
}

void CModel::Free()
{

}
