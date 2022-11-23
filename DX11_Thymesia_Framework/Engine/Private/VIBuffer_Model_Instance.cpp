#include "VIBuffer_Model_Instance.h"
#include "GameInstance.h"
#include "ModelData.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "GameObject.h"
#include "Shader.h"

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

void CVIBuffer_Model_Instance::Culling_Instance(vector<INSTANCE_MESH_DESC>& In_ParticleDescs, const _float In_fRange)
{
	shared_ptr<CGameInstance> pGameInstance = GAMEINSTANCE;
	list<INSTANCE_MESH_DESC*> pVisibleInstanceDescs;

	_vector vPosition;
	for (auto& elem : In_ParticleDescs)
	{
		vPosition = XMLoadFloat3(&elem.vTarnslation);
		vPosition = XMVectorSetW(vPosition, 1.f);
		if (pGameInstance->isIn_Frustum_InWorldSpace(vPosition, In_fRange))
		{
			pVisibleInstanceDescs.push_back(&elem);
		}
	}

	m_iVisibleCount = pVisibleInstanceDescs.size();

	if (0 == m_iVisibleCount)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	_matrix							WorldMatrix;
	_uint							iIndex = 0;
	DEVICECONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (auto& elem : pVisibleInstanceDescs)
	{
		WorldMatrix = (*elem).Get_Matrix();

		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[iIndex].vRight, WorldMatrix.r[0]);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[iIndex].vUp, WorldMatrix.r[1]);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[iIndex].vLook, WorldMatrix.r[2]);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[iIndex].vTranslation, WorldMatrix.r[3]);
		++iIndex;
	}

	DEVICECONTEXT->Unmap(m_pVBInstance.Get(), 0);

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
}

void CVIBuffer_Model_Instance::Init_Instance(_uint In_iNumInstance)
{
	m_pVBInstance.Reset();

	m_iInstanceStride		= sizeof(VTXMODELINSTANCE);
	m_iNumInstance			= In_iNumInstance;
	m_iVisibleCount		= m_iNumInstance;
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
		pModelInstance[i].vRight			= _float4(1.f, 0.f, 0.f, 0.f);
		pModelInstance[i].vUp			= _float4(0.f, 1.f, 0.f, 0.f);
		pModelInstance[i].vLook			= _float4(0.f, 0.f, 1.f, 0.f);
		pModelInstance[i].vTranslation   = _float4(0.f, 0.f, 0.f, 1.f);
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
			char	szFullPath[MAX_PATH] = "";
			string	strPath;

			strPath = m_pModelData->Material_Datas[i]->szTextureName[j];

			char	szFileName[MAX_PATH] = "";
			char	szExt[MAX_PATH] = "";

			_splitpath_s(strPath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szTextureFilePath, MAX_PATH);

			GAMEINSTANCE->Load_Textures(szFileName, szTextureFilePath, MEMORY_TYPE::MEMORY_STATIC);
			Material.pTextures[j] = m_pOwner.lock()->Add_Component<CTexture>();
			Material.pTextures[j].lock().get()->Use_Texture(szFileName);
		}

		m_Materials.push_back(Material);
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
		}

		return S_OK;
	}

	return m_Materials[iMaterialIndex].pTextures[eActorType].lock()->Set_ShaderResourceView(pShader, pConstantName);
}


HRESULT CVIBuffer_Model_Instance::Render()
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


HRESULT CVIBuffer_Model_Instance::Render_Mesh(_uint iMeshContainerIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

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

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	DEVICECONTEXT->DrawIndexedInstanced(m_MeshContainers[iMeshContainerIndex].lock()->Get_NumIndices(), m_iVisibleCount, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Model_Instance::Update(const vector<INSTANCE_MESH_DESC>& In_ParticleDescs)
{
	if (0 == In_ParticleDescs.size() || 0 == m_iNumInstance || In_ParticleDescs.size() > In_ParticleDescs.size())
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICECONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&In_ParticleDescs[i].vRotation));

		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vRight      , RotationMatrix.r[0] * In_ParticleDescs[i].vScale.x);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vUp         , RotationMatrix.r[1] * In_ParticleDescs[i].vScale.y);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vLook       , RotationMatrix.r[2] * In_ParticleDescs[i].vScale.z);
		XMStoreFloat4(&((VTXMODELINSTANCE*)SubResource.pData)[i].vTranslation, XMVectorSetW(XMLoadFloat3(&In_ParticleDescs[i].vTarnslation), 1.f));
	}

	DEVICECONTEXT->Unmap(m_pVBInstance.Get(), 0);
}

void CVIBuffer_Model_Instance::OnDestroy()
{
	
}

void CVIBuffer_Model_Instance::Free()
{
}
