#include "VIBuffer_Model_Instance.h"
#include "GameInstance.h"
#include "ModelData.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "GameObject.h"
#include "Shader.h"
#include "SMath.h"
#include "Collider.h"

GAMECLASS_C(CVIBuffer_Model_Instance)
CLONE_C(CVIBuffer_Model_Instance, CComponent)

HRESULT CVIBuffer_Model_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Model_Instance::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Model_Instance::Start()
{
}

void CVIBuffer_Model_Instance::Init_Model(const char* In_szModelName)
{
	Reset_Model();

	m_pModelData = GAMEINSTANCE->Get_ModelFromKey(In_szModelName);

	if (!m_pModelData.get())
		return;

	m_szModelKey = In_szModelName;

	char szDir[MAX_PATH];
	_splitpath_s(m_pModelData->szModelFilePath.c_str(), nullptr, 0, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
	
	Create_Materials(szDir);
	Create_MeshContainers();

	m_fMaxOffsetRange = m_pModelData->Get_MaxOffsetRange();
}

void CVIBuffer_Model_Instance::Init_Instance(_uint In_iNumInstance)
{
	m_pVBInstance.Reset();

	m_iInstanceStride		= sizeof(VTXMODELINSTANCE);
	m_iNumInstance			= In_iNumInstance;
	m_iVisibleCount		    = m_iNumInstance;
	m_iNumVertexBuffers		= 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth				= m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride	= m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags				= 0;

	VTXMODELINSTANCE* pModelInstance = DBG_NEW VTXMODELINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pModelInstance[i].vRight	    = _float4(1.f, 0.f, 0.f, 0.f);
		pModelInstance[i].vUp			= _float4(0.f, 1.f, 0.f, 0.f);
		pModelInstance[i].vLook			= _float4(0.f, 0.f, 1.f, 0.f);
		pModelInstance[i].vTranslation  = _float4(0.f, 0.f, 0.f, 1.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pModelInstance;

	DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance);
	
	Safe_Delete_Array(pModelInstance);
}

void CVIBuffer_Model_Instance::Reset_Model() 
{
	for (auto& iter : m_MeshContainers)
		iter.lock()->OnDestroy();

	for (auto& iter : m_Materials)
	{
		for (_uint i = 0; i < (_uint)AI_TEXTURE_TYPE_MAX; ++i)
		{
			if (!iter.pTextures[i].lock())
				continue;

			iter.pTextures[i].lock()->OnDestroy();
		}
	}

	m_pModelData.reset();
	m_iNumMeshContainers	= 0;
	m_iNumMaterials			= 0;
	m_MeshContainers.clear();
	m_Materials.clear();
}


void CVIBuffer_Model_Instance::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pModelData->iNumMeshs;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		weak_ptr<CMeshContainer> pMeshContainer  = Get_Owner().lock()->Add_Component<CMeshContainer>();
		pMeshContainer.lock()->Init_Mesh(m_pModelData->Mesh_Datas[i]);
		m_MeshContainers.push_back(pMeshContainer);
	}
}

void CVIBuffer_Model_Instance::Create_Materials(const char* pModelFilePath)
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
				if (j == (_uint)aiTextureType_SPECULAR && MODEL_TYPE::NONANIM == m_pModelData->eModelType)
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

void CVIBuffer_Model_Instance::Create_ORM_Material(MODEL_MATERIAL& Out_Material, const _uint In_iMaterialIndex, const _char* pModelFilePath)
{
	string			szFullPath;

	string		strPath;

	strPath = m_pModelData->Material_Datas[In_iMaterialIndex]->szTextureName[(_uint)aiTextureType_NORMALS];


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

HRESULT CVIBuffer_Model_Instance::Bind_SRV(weak_ptr<CShader> pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eActorType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex].lock()->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (!m_Materials[iMaterialIndex].pTextures[eActorType].lock().get())
	{
		if (aiTextureType::aiTextureType_DIFFUSE == eActorType)
		{
			vector<ComPtr<ID3D11ShaderResourceView>> NullTexture = GAMEINSTANCE->Get_TexturesFromKey("NullTexture");

			pShader.lock()->Set_ShaderResourceView(pConstantName, NullTexture.front());
			return S_OK;
		}

		return E_FAIL;

	}

	return m_Materials[iMaterialIndex].pTextures[eActorType].lock()->Set_ShaderResourceView(pShader, pConstantName);
}


HRESULT CVIBuffer_Model_Instance::Render(ID3D11DeviceContext* pDeviceContext)
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
		m_pVBInstance.Get()
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	DEVICECONTEXT->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	DEVICECONTEXT->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	DEVICECONTEXT->IASetPrimitiveTopology(m_eToplogy);

	DEVICECONTEXT->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}


HRESULT CVIBuffer_Model_Instance::Render_Mesh(_uint iMeshContainerIndex, ID3D11DeviceContext* pDeviceContext)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	if (0 == m_iVisibleCount)
		return E_FAIL;

	/*if (sizeof(m_pVisibleInstanceDescs[m_iCurrentVisibleIndex]) / sizeof(INSTANCE_MESH_DESC))
		return E_FAIL;*/

	ID3D11Buffer* pVertexBuffers[] = {
		m_MeshContainers[iMeshContainerIndex].lock()->Get_VertexBuffer().Get(),
		m_pVBInstance.Get()
	};

	_uint		iStrides[] = {
		m_MeshContainers[iMeshContainerIndex].lock()->Get_Stride(),
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	DEVICECONTEXT->IASetVertexBuffers(0,m_iNumVertexBuffers , pVertexBuffers, iStrides, iOffsets);
	DEVICECONTEXT->IASetIndexBuffer(m_MeshContainers[iMeshContainerIndex].lock()->Get_IndexBuffer().Get(), m_MeshContainers[iMeshContainerIndex].lock()->Get_IndexFormat(), 0);
	DEVICECONTEXT->IASetPrimitiveTopology(m_MeshContainers[iMeshContainerIndex].lock()->Get_Topology());

	DEVICECONTEXT->DrawIndexedInstanced(m_MeshContainers[iMeshContainerIndex].lock()->Get_NumIndices(), m_iVisibleCount, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Model_Instance::Culling_Instance(vector<INSTANCE_MESH_DESC>& In_ParticleDescs, const _float In_fRatio)
{
	if (In_ParticleDescs.empty() || m_bCulling)
		return;

	//_int iUpdateIndex = 1 - m_iCurrentVisibleIndex;

	shared_ptr<CGameInstance> pGameInstance = GAMEINSTANCE;

	_uint iIndex = 0;

	for (auto& elem : In_ParticleDescs)
	{
		if (pGameInstance->isIn_Frustum_InWorldSpace(XMVectorSetW(XMLoadFloat3(&elem.vCenter), 1.f), elem.fMaxRange * In_fRatio))
		{
			m_pVisibleInstanceDescs.push_back(elem);
		}
	}

	m_iVisibleCount        = m_pVisibleInstanceDescs.size();
	m_bCulling             = true;

	//Update_VisibleInstance();

	/*shared_ptr<CGameInstance> pGameInstance = GAMEINSTANCE;

	_int iCount = 0;

	sort(m_pModelInstance,
		m_pModelInstance + m_iNumInstance,
		[&pGameInstance, &In_fRange, &iCount](VTXMODELINSTANCE& Left, VTXMODELINSTANCE& Right) {

			XMVECTOR LeftFromVector, RightFromVector;
			LeftFromVector = XMLoadFloat4(&Left.vTranslation);
			RightFromVector = XMLoadFloat4(&Right.vTranslation);
			_bool IsInLeft = pGameInstance->isIn_Frustum_InWorldSpace(LeftFromVector, In_fRange);
			_bool IsInRight = pGameInstance->isIn_Frustum_InWorldSpace(RightFromVector, In_fRange);
			if (IsInLeft == IsInRight)
			{
				return false;
			}
			else if (IsInLeft && !IsInRight)
			{
				iCount++;
				return true;
			}
		}
		);

	m_iVisiableCount = iCount;

	if (m_iVisiableCount > m_iNumInstance)
	{
		DEBUG_ASSERT;
	}*/
}

void CVIBuffer_Model_Instance::Update(vector<INSTANCE_MESH_DESC>& In_ParticleDescs, const _bool In_bUseCulling)
{
	if (0 == In_ParticleDescs.size() || 0 == m_iNumInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	ID3D11DeviceContext* pDeviceContext = GAMEINSTANCE->Get_BeforeRenderContext();

	pDeviceContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&In_ParticleDescs[i].vRotation));

		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vRight      , RotationMatrix.r[0] * In_ParticleDescs[i].vScale.x);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vUp         , RotationMatrix.r[1] * In_ParticleDescs[i].vScale.y);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vLook       , RotationMatrix.r[2] * In_ParticleDescs[i].vScale.z);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vTranslation, XMVectorSetW(XMLoadFloat3(&In_ParticleDescs[i].vTarnslation), 1.f));
	}

	pDeviceContext->Unmap(m_pVBInstance.Get(), 0);

	GAMEINSTANCE->Release_BeforeRenderContext(pDeviceContext);
}

void CVIBuffer_Model_Instance::Update_VisibleInstance(ID3D11DeviceContext* pDeviceContext)
{
	if (0 == m_iNumInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	_matrix							WorldMatrix;

	pDeviceContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_int i = 0; i < m_iNumInstance; ++i)
	{
		if (i < m_iVisibleCount)
		{
			WorldMatrix = m_pVisibleInstanceDescs[i].Get_Matrix();
		}
		else
		{
			WorldMatrix = XMMatrixIdentity();
		}

		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vRight, WorldMatrix.r[0]);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vUp, WorldMatrix.r[1]);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vLook, WorldMatrix.r[2]);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vTranslation, WorldMatrix.r[3]);
	}

	pDeviceContext->Unmap(m_pVBInstance.Get(), 0);

	m_pVisibleInstanceDescs.clear();
	m_bCulling = false;
}

void CVIBuffer_Model_Instance::OnDestroy()
{

}

void CVIBuffer_Model_Instance::Free()
{
}
