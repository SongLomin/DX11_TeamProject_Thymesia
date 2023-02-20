#include "Resource_Manager.h"
#include "GameInstance.h"
#include "ModelData.h"
#include "Shader.h"
#include "SubThread_Pool.h"
#include "SoundManager.h"

IMPLEMENT_SINGLETON(CResource_Manager)

static ::recursive_mutex Load_texture_Mutex;
static ::recursive_mutex Load_Model_Mutex;
static ::mutex Load_Shader_Mutex;
static ::mutex Write_ShaderLog_Mutex;

HRESULT CResource_Manager::Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType)
{
	unique_lock<::recursive_mutex> lock(Load_texture_Mutex);

	_tchar szTextureFilePath[MAX_PATH] = TEXT("");

	_hashcode KeyToHash = hash<string>()(_strKey);

	auto iter = m_SRVs[(_uint)eMemType].find(KeyToHash);
	//auto iter = find_if(m_SRVs[(_uint)eMemType].begin(), m_SRVs[(_uint)eMemType].end(), CTag_Finder_c_str(_strKey));

	if (m_SRVs[(_uint)eMemType].end() != iter)
		return S_OK;

	

	HRESULT hr(0);
	_tchar szExt[MAX_PATH] = TEXT("");
	_bool bCreated = false;

	ScratchImage TextureImage;

	for (_uint i(0); i < 1024; ++i)
	{
		ComPtr<ID3D11ShaderResourceView> pSRV;
		if (lstrcmpW(szTextureFilePath, pTextureFilePath) == 0)
		{
			m_TextureFilePaths[(_uint)eMemType][_strKey].szFilePath = std::filesystem::path(pTextureFilePath).string();
			m_TextureFilePaths[(_uint)eMemType][_strKey].MipMapLevels = 0;
			return S_OK;
		}

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		ZeroMemory(szExt, sizeof(_tchar) * MAX_PATH);
		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		if (!lstrcmp(szExt, TEXT(".dds")))
		{
			hr = CreateDDSTextureFromFile(DEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());


		}
		else
		{
			hr = CreateWICTextureFromFile(DEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());

		}
		if (FAILED(hr))
		{
			if (bCreated)
			{
				m_TextureFilePaths[(_uint)eMemType][_strKey].szFilePath = std::filesystem::path(pTextureFilePath).string();
				m_TextureFilePaths[(_uint)eMemType][_strKey].MipMapLevels = 0;
				return S_OK;
			}
			else
				return E_FAIL;
		}

		m_SRVs[(_uint)eMemType][KeyToHash].emplace_back(pSRV);
		bCreated = true;
	}


	assert(false);
	return E_FAIL;
}

//HRESULT CResource_Manager::Load_Textures_Generate_MipMaps(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType, const _int In_iMipMapLevels)
//{
//	_tchar szTextureFilePath[MAX_PATH] = TEXT("");
//
//	_hashcode KeyToHash = hash<string>()(_strKey);
//
//	auto iter = m_SRVs[(_uint)eMemType].find(KeyToHash);
//
//	if (m_SRVs[(_uint)eMemType].end() != iter)
//		return S_OK;
//
//	HRESULT hr(0);
//	_tchar szExt[MAX_PATH] = TEXT("");
//	tstring	szNewDDSPath;
//	_bool bCreated = false;
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC DataDesc;
//
//
//	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
//
//
//	for (_uint i(0); i < 1024; ++i)
//	{
//		ComPtr<ID3D11ShaderResourceView> pSRV;
//		ComPtr<ID3D11Resource> pResource;
//		if (lstrcmpW(szTextureFilePath, pTextureFilePath) == 0)
//		{
//			m_TextureFilePaths[(_uint)eMemType][_strKey].szFilePath = std::filesystem::path(pTextureFilePath).string();
//			m_TextureFilePaths[(_uint)eMemType][_strKey].MipMapLevels = 8;
//			return S_OK;
//		}
//
//		wsprintf(szTextureFilePath, pTextureFilePath, i);
//		ZeroMemory(szExt, sizeof(_tchar) * MAX_PATH);
//		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);
//
//		if (!lstrcmp(szExt, TEXT(".dds")))
//		{
//			hr = CreateDDSTextureFromFile(DEVICE, szTextureFilePath, pResource.GetAddressOf(), pSRV.GetAddressOf());
//		}
//		else
//		{
//			hr = CreateWICTextureFromFile(DEVICE, szTextureFilePath, pResource.GetAddressOf(), pSRV.GetAddressOf());
//		}
//
//		if (FAILED(hr))
//		{
//			if (bCreated)
//			{
//				m_TextureFilePaths[(_uint)eMemType][_strKey].szFilePath = std::filesystem::path(pTextureFilePath).string();
//				m_TextureFilePaths[(_uint)eMemType][_strKey].MipMapLevels = 8;
//				return S_OK;
//			}
//			else
//				return E_FAIL;
//		}
//
//		ComPtr<ID3D11Texture2D> pTexture2D;
//		HRESULT Casting_hr = pResource->QueryInterface(IID_ID3D11Texture2D, &pTexture2D);
//		if (FAILED(Casting_hr)) {
//			DEBUG_ASSERT;
//		}
//
//		// 밉맵 생성
//		ComPtr<ID3D11Texture2D> tex;
//		D3D11_TEXTURE2D_DESC texDesc;
//		pTexture2D->GetDesc(&texDesc);
//		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//		texDesc.CPUAccessFlags = 0;
//		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		texDesc.MipLevels = 11;
//		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
//		texDesc.Usage = D3D11_USAGE_DEFAULT;
//
//		if (SUCCEEDED(DEVICE->CreateTexture2D(&texDesc, nullptr, &tex)))
//		{
//			ComPtr<ID3D11Texture2D> stagTex; // create Staging texture_____________
//			D3D11_TEXTURE2D_DESC stagDesc;
//			pTexture2D->GetDesc(&stagDesc);
//			stagDesc.ArraySize = 1;
//			stagDesc.BindFlags = 0;
//			stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//			stagDesc.MipLevels = 1;
//			stagDesc.MiscFlags = 0;
//			stagDesc.Usage = D3D11_USAGE_STAGING;
//
//			if (SUCCEEDED(DEVICE->CreateTexture2D(&stagDesc, nullptr, &stagTex)))
//			{
//				DEVICECONTEXT->CopyResource(stagTex.Get(), pTexture2D.Get());
//				D3D11_MAPPED_SUBRESOURCE mapped;
//				DEVICECONTEXT->Map(stagTex.Get(), 0, D3D11_MAP_READ, 0, &mapped);
//				UINT* arr = DBG_NEW UINT[(mapped.RowPitch / (float)sizeof(UINT)) * stagDesc.Height];
//				ZeroMemory(arr, mapped.RowPitch * stagDesc.Height);
//				CopyMemory(arr, mapped.pData, mapped.RowPitch * stagDesc.Height);
//				DEVICECONTEXT->Unmap(stagTex.Get(), 0);
//
//
//				DEVICECONTEXT->UpdateSubresource(tex.Get(), 0, &CD3D11_BOX(0, 0, 0, stagDesc.Width, stagDesc.Height, 1), arr, mapped.RowPitch, mapped.DepthPitch);
//
//				// create SRV of mipmap texture______________________________________________________________
//				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//				srvDesc.Format = texDesc.Format;
//				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//				srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
//				srvDesc.Texture2D.MostDetailedMip = 0;
//
//				ComPtr<ID3D11ShaderResourceView> pMipMapSRV;
//
//				hr = DEVICE->CreateShaderResourceView(tex.Get(), &srvDesc, pMipMapSRV.GetAddressOf());
//
//				if (FAILED(hr))
//				{
//					DEBUG_ASSERT;
//				}
//
//				DEVICECONTEXT->GenerateMips(pMipMapSRV.Get());
//
//				ComPtr<ID3D11Resource> pNewMipMapResource;
//
//				pMipMapSRV->GetResource(pNewMipMapResource.GetAddressOf());
//
//				ComPtr<ID3D11Texture2D> pNewMipMapTexture;
//				Casting_hr = pNewMipMapResource->QueryInterface(IID_ID3D11Texture2D, &pNewMipMapTexture);
//				if (FAILED(Casting_hr)) {
//					DEBUG_ASSERT;
//				}
//
//				D3D11_TEXTURE2D_DESC NewMipMapDesc;
//				pNewMipMapTexture->GetDesc(&NewMipMapDesc);
//
//				Safe_Delete_Array(arr);
//
//				m_SRVs[(_uint)eMemType][KeyToHash].emplace_back(pMipMapSRV);
//			}
//		}
//
//
//		bCreated = true;
//	}
//
//	assert(false);
//	return E_FAIL;
//}

HRESULT CResource_Manager::Generate_MipMap(const _tchar* pTextureFilePath, const _int In_iMipMapLevels)
{
	_tchar szTextureFilePath[MAX_PATH] = TEXT("");

	//_hashcode KeyToHash = hash<string>()(_strKey);

	//auto iter = m_SRVs[(_uint)eMemType].find(KeyToHash);
	//auto iter = find_if(m_SRVs[(_uint)eMemType].begin(), m_SRVs[(_uint)eMemType].end(), CTag_Finder_c_str(_strKey));

	/*if (m_SRVs[(_uint)eMemType].end() != iter)
		return S_OK;*/

	HRESULT hr(0);
	_tchar szExt[MAX_PATH] = TEXT("");
	tstring	szNewDDSPath;
	_bool bCreated = false;

	D3D11_SHADER_RESOURCE_VIEW_DESC DataDesc;


	//hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);


	for (_uint i(0); i < 1024; ++i)
	{
		ComPtr<ID3D11ShaderResourceView> pSRV;
		ComPtr<ID3D11Resource> pResource;
		if (lstrcmpW(szTextureFilePath, pTextureFilePath) == 0)
		{
			return S_OK;
		}

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		ZeroMemory(szExt, sizeof(_tchar) * MAX_PATH);
		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		if (!lstrcmp(szExt, TEXT(".dds")))
		{
			hr = CreateDDSTextureFromFile(DEVICE, szTextureFilePath, pResource.GetAddressOf(), pSRV.GetAddressOf());
			ComPtr<ID3D11Texture2D> pTexture2D;
			HRESULT Casting_hr = pResource->QueryInterface(IID_ID3D11Texture2D, &pTexture2D);
			if (FAILED(Casting_hr)) {
				DEBUG_ASSERT;
			}

			D3D11_TEXTURE2D_DESC texDesc;
			pTexture2D->GetDesc(&texDesc);
			_int i = 0;
		}
		else
		{
			ScratchImage image;
			ScratchImage mipChain;
			hr = LoadFromWICFile(pTextureFilePath, WIC_FLAGS_NONE, nullptr, image);

			hr = GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

			if (FAILED(hr))
			{
				DEBUG_ASSERT;
			}

			szNewDDSPath = szTextureFilePath;
			szNewDDSPath = szNewDDSPath.substr(0, szNewDDSPath.size() - 4);
			szNewDDSPath += TEXT(".dds");

			hr = SaveToDDSFile(mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(),
				DDS_FLAGS_NONE, szNewDDSPath.c_str());

			if (FAILED(hr))
			{
				DEBUG_ASSERT;
			}
			
		}

		if (FAILED(hr))
		{
			if (bCreated)
			{
				return S_OK;
			}
			else
				return E_FAIL;
		}

		bCreated = true;
	}

	assert(false);
	return E_FAIL;
}

vector<ComPtr<ID3D11ShaderResourceView>> CResource_Manager::Get_TexturesFromKey(const _char* _Str_Key, MEMORY_TYPE _eType)
{

	_hashcode KeyToHash = hash<string>()(_Str_Key);

	if (MEMORY_TYPE::MEMORY_END == _eType)
	{
		auto	iter = m_SRVs[(_uint)MEMORY_TYPE::MEMORY_STATIC].find(KeyToHash);
		//auto	iter = find_if(m_SRVs[(_uint)MEMORY_TYPE::MEMORY_STATIC].begin(), m_SRVs[(_uint)MEMORY_TYPE::MEMORY_STATIC].end(), CTag_Finder_c_str(_Str_Key));

		if (m_SRVs[(_uint)MEMORY_TYPE::MEMORY_STATIC].end() != iter)
		{
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_STATIC]["Textures"][_Str_Key]["FilePath"] = m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_STATIC][_Str_Key].szFilePath;
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_STATIC]["Textures"][_Str_Key]["MipMapLevels"] = m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_STATIC][_Str_Key].MipMapLevels;
			return (*iter).second;
		}

		iter = m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].find(KeyToHash);
		//iter = find_if(m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].begin(), m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].end(), CTag_Finder_c_str(_Str_Key));

		if (m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].end() != iter)
		{
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_DYNAMIC]["Textures"][_Str_Key]["FilePath"] = m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC][_Str_Key].szFilePath;
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_DYNAMIC]["Textures"][_Str_Key]["MipMapLevels"] = m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC][_Str_Key].MipMapLevels;
			return (*iter).second;
		}
	}
	else
	{
		auto	iter = m_SRVs[(_uint)_eType].find(KeyToHash);

		if (m_SRVs[(_uint)_eType].end() != iter)
		{
			m_UsingResourceJson[(_int)_eType]["Textures"][_Str_Key]["FilePath"] = m_TextureFilePaths[(_int)_eType][_Str_Key].szFilePath;
			m_UsingResourceJson[(_int)_eType]["Textures"][_Str_Key]["MipMapLevels"] = m_TextureFilePaths[(_int)_eType][_Str_Key].MipMapLevels;
			return (*iter).second;
		}
	}

	//텍스처를 못찾음.
	//MEMORY_TYPE을 확인하거나 키를 다시 봐볼 것.
	//assert(false);
	return vector<ComPtr<ID3D11ShaderResourceView>>();
}

HRESULT CResource_Manager::Release_ResourceByMemoryType(MEMORY_TYPE _eMemType)
{
	if (MEMORY_TYPE::MEMORY_END == _eMemType)
	{
		DEBUG_ASSERT;
	}

	//Release Textures
	for (auto& pair : m_SRVs[(_uint)_eMemType])
	{
		for (auto& elem : pair.second)
		{
			elem.Reset();
		}

		pair.second.clear();
	}

	m_TextureFilePaths[(_uint)_eMemType].clear();

	m_SRVs[(_uint)_eMemType].clear();

	for (auto& pair : m_pScenes[(_uint)_eMemType])
	{
		pair.second->OnDestroy();
		pair.second.reset();
	}

	m_pScenes[(_uint)_eMemType].clear();

	m_UsingResourceJson[(_uint)_eMemType].clear();


	return S_OK;
}

void CResource_Manager::Write_JsonUsingResource(const char* In_szFilePath)
{
	if (m_UsingResourceJson[(_uint)MEMORY_TYPE::MEMORY_STATIC].find("Sounds") != m_UsingResourceJson[(_uint)MEMORY_TYPE::MEMORY_STATIC].end())
	{
		m_UsingResourceJson[(_uint)MEMORY_TYPE::MEMORY_STATIC]["Sounds"].clear();
	}
	
	GET_SINGLE(CSound_Manager)->Write_JsonUsingResource(m_UsingResourceJson[(_uint)MEMORY_TYPE::MEMORY_STATIC]);

	if (FAILED(CJson_Utility::Save_Json(In_szFilePath, m_UsingResourceJson)))
	{
		DEBUG_ASSERT;
	}
}

void CResource_Manager::Load_ResourcesFromJson(const char* In_szFilePath)
{
	json In_Json;

	if (FAILED(CJson_Utility::Load_Json(In_szFilePath, In_Json)))
	{
		DEBUG_ASSERT;
		return;
	}

	shared_ptr<CSubThread_Pool> pSubThreads = CSubThread_Pool::Create(GET_SINGLE(CThread_Manager)->Get_NumThread());

	for (_int i = 0; i < (_int)MEMORY_TYPE::MEMORY_END; ++i)
	{
		pSubThreads->Enqueue_Job(bind(&CResource_Manager::Load_ModelResourcesFromJson, this, placeholders::_1, placeholders::_2),
			In_Json[i]["Model"], (MEMORY_TYPE)i);

		pSubThreads->Enqueue_Job(bind(&CResource_Manager::Load_TextureResourcesFromJson, this, placeholders::_1, placeholders::_2),
			In_Json[i]["Textures"], (MEMORY_TYPE)i);
	}


	for (auto& elem : In_Json[(_uint)MEMORY_TYPE::MEMORY_STATIC]["Sounds"])
	{
		GET_SINGLE(CSound_Manager)->LoadSoundFile(elem);
	}
	

	//list<future<void>>	Threads;

	//for (_int i = 0; i < 2; ++i)
	//{
	//	/*Load_ModelResourcesFromJson(In_Json[i]["Model"], (MEMORY_TYPE)i);
	//	Load_TextureResourcesFromJson(In_Json[i]["Textures"], (MEMORY_TYPE)i);*/

	//	Threads.push_back(async(launch::async,
	//		bind(&CResource_Manager::Load_ModelResourcesFromJson, this, placeholders::_1, placeholders::_2),
	//		In_Json[i]["Model"], (MEMORY_TYPE)i));

	//	Threads.push_back(async(launch::async,
	//		bind(&CResource_Manager::Load_TextureResourcesFromJson, this, placeholders::_1, placeholders::_2),
	//		In_Json[i]["Textures"], (MEMORY_TYPE)i));
	//}

	//for (auto& elem : Threads)
	//{
	//	elem.get();
	//}

	pSubThreads->Wait_JobDone();
	pSubThreads->OnDestroy();
	pSubThreads.reset();
}

void CResource_Manager::Load_ModelResourcesFromJson(json& In_ModelJson, const MEMORY_TYPE In_eMemoryType)
{
	_float4x4	TransformMatrix;
	string		FilePath;
	_int		ModelType;

	for (auto json_iter : In_ModelJson.items())
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				TransformMatrix.m[i][j] = json_iter.value()["TransformMatrix"][i][j];
			}
		}

		FilePath = json_iter.value()["FilePath"];
		ModelType = json_iter.value()["ModelType"];

		Load_Model(json_iter.key().c_str(), FilePath.c_str(),
			(MODEL_TYPE)ModelType, XMLoadFloat4x4(&TransformMatrix), In_eMemoryType);
	}
}

void CResource_Manager::Load_TextureResourcesFromJson(json& In_TextureJson, const MEMORY_TYPE In_eMemoryType)
{
	_float4x4	TransformMatrix;
	string		FilePath;
	_int		ModelType;
	tstring		szTexturePath;
	_int		MipMapLevels;

	for (auto json_iter : In_TextureJson.items())
	{
		FilePath = json_iter.value()["FilePath"];
		MipMapLevels = json_iter.value()["MipMapLevels"];

		szTexturePath = std::filesystem::path(FilePath).wstring();

		Load_Textures(json_iter.key().c_str(), szTexturePath.c_str(), In_eMemoryType);
	}
}

HRESULT CResource_Manager::Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType, _bool bAnimZero)
{
	shared_ptr<MODEL_DATA> TempModel = make_shared<MODEL_DATA>();

	/*char			szFullPath[MAX_PATH] = "";
	strcpy_s(szFullPath, sModelFilePath);*/



	if (FAILED(TempModel->Make_ModelData(sModelFilePath, eModelType, In_TransformMatrix, bAnimZero)))
	{
		return E_FAIL;
	}

	
	unique_lock<::recursive_mutex> lock(Load_Model_Mutex);
	

	auto iter = m_pScenes[(_uint)eMemType].find(sKey);
	//기존에 이미 같은 키로 등록된 모델이 있음.
	if (m_pScenes[(_uint)eMemType].end() != iter)
		return E_FAIL;

	

	m_pScenes[(_uint)eMemType].emplace(sKey, TempModel);

//	char szDir[MAX_PATH];
//	_splitpath_s(TempModel->szModelFilePath.c_str(), nullptr, 0, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
//
//	_int iNumMaterials = TempModel->iNumMaterials;
//
//#ifdef _DEBUG
//	cout << sKey << endl;
//	cout << "========================================" << endl;
//	cout << "NumMaterials: " << iNumMaterials << endl;
//#endif // _DEBUG
//
//
//
//
//	for (_uint i = 0; i < iNumMaterials; ++i)
//	{
//		for (_uint j = 0; j < (_uint)AI_TEXTURE_TYPE_MAX; ++j)
//		{
//			char			szFullPath[MAX_PATH] = "";
//
//			string		strPath;
//
//			//if (FAILED(AISCENE->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
//			//	continue;
//
//			strPath = TempModel->Material_Datas[i]->szTextureName[j];
//
//
//			char			szFileName[MAX_PATH] = "";
//			char			szExt[MAX_PATH] = "";
//
//			_splitpath_s(strPath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
//
//
//
//			strcpy_s(szFullPath, szDir);
//			strcat_s(szFullPath, szFileName);
//			strcat_s(szFullPath, szExt);
//
//			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
//			//_tchar		szTextureKey[MAX_PATH] = TEXT("");
//
//			MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szTextureFilePath, MAX_PATH);
//			//MultiByteToWideChar(CP_ACP, 0, szFileName, (_int)strlen(szFileName), szTextureKey, MAX_PATH);
//
//#ifdef _DEBUG
//			//cout << "Load_Texture: " << szFullPath << endl;
//#endif // _DEBUG
//
//			GAMEINSTANCE->Load_Textures(szFileName, szTextureFilePath, MEMORY_TYPE::MEMORY_STATIC);
//		}
//	}

	return S_OK;
}

shared_ptr<MODEL_DATA> CResource_Manager::Get_ModelFromKey(const _char* _sKey, MEMORY_TYPE _eType)
{

	if (MEMORY_TYPE::MEMORY_END == _eType)
	{
		auto	iter = find_if(m_pScenes[(_uint)MEMORY_TYPE::MEMORY_STATIC].begin(), m_pScenes[(_uint)MEMORY_TYPE::MEMORY_STATIC].end(), CTag_Finder_c_str(_sKey));

		if (m_pScenes[(_uint)MEMORY_TYPE::MEMORY_STATIC].end() != iter)
		{
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_STATIC]["Model"][_sKey]["TransformMatrix"] = iter->second->TransformMatrix.m;
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_STATIC]["Model"][_sKey]["FilePath"] = iter->second->szModelFilePath;
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_STATIC]["Model"][_sKey]["ModelType"] = (_int)iter->second->eModelType;
			return (*iter).second;
		}

		iter = find_if(m_pScenes[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].begin(), m_pScenes[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].end(), CTag_Finder_c_str(_sKey));

		if (m_pScenes[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].end() != iter)
		{
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_DYNAMIC]["Model"][_sKey]["TransformMatrix"] = iter->second->TransformMatrix.m;
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_DYNAMIC]["Model"][_sKey]["FilePath"] = iter->second->szModelFilePath;
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_DYNAMIC]["Model"][_sKey]["ModelType"] = (_int)iter->second->eModelType;
			return (*iter).second;
		}
	}
	else
	{
		auto	iter = find_if(m_pScenes[(_uint)_eType].begin(), m_pScenes[(_uint)_eType].end(), CTag_Finder_c_str(_sKey));

		if (m_pScenes[(_uint)_eType].end() != iter)
		{
			m_UsingResourceJson[(_int)_eType]["Model"][_sKey]["TransformMatrix"] = iter->second->TransformMatrix.m;
			m_UsingResourceJson[(_int)_eType]["Model"][_sKey]["FilePath"] = iter->second->szModelFilePath;
			m_UsingResourceJson[(_int)_eType]["Model"][_sKey]["ModelType"] = (_int)iter->second->eModelType;
			return (*iter).second;
		}
	}

	return shared_ptr<MODEL_DATA>();
}

vector<const string*> CResource_Manager::Get_AllModelKeys()
{
	vector<const string*> ResultKeys;

	for (auto& elem_List : m_pScenes)
	{
		for (auto& elem : elem_List)
		{
			ResultKeys.push_back(&(elem.first));
		}
	}

	return ResultKeys;
}

vector<const string*> CResource_Manager::Get_AllNoneAnimModelKeys()
{
	vector<const string*> ResultKeys;

	for (auto& elem_List : m_pScenes)
	{
		for (auto& elem : elem_List)
		{
			if (MODEL_TYPE::NONANIM == elem.second->eModelType)
				ResultKeys.push_back(&elem.first);
		}
	}

	return ResultKeys;
}

vector<const string*> CResource_Manager::Get_AllAnimModelKeys()
{
	vector<const string*> ResultKeys;

	for (auto& elem_List : m_pScenes)
	{
		for (auto& elem : elem_List)
		{
			if(MODEL_TYPE::ANIM == elem.second->eModelType)
				ResultKeys.push_back(&elem.first);
		}
	}

	return ResultKeys;
}

HRESULT CResource_Manager::Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath)
{
	if (FAILED(Load_Shader_Internal(sKey, sShaderFilePath)))
	{
		return E_FAIL;
	}
	

#ifdef _DEBUG
	unique_lock<::mutex> lock(Write_ShaderLog_Mutex);

	_hashcode KeyToHashcode = hash<tstring>()(sKey);

	auto iter = m_ShaderFilePaths.find(KeyToHashcode);

	if (iter == m_ShaderFilePaths.end())
	{
		m_ShaderFilePaths.emplace(KeyToHashcode, SHADER_FILEPATH());
		m_ShaderFilePaths[KeyToHashcode].szKey = sKey;
		m_ShaderFilePaths[KeyToHashcode].szFilePath = sShaderFilePath;
		m_ShaderFilePaths[KeyToHashcode].iFileSize = filesystem::file_size(filesystem::path(sShaderFilePath));
	}



#endif // _DEBUG


	return S_OK;
}

HRESULT CResource_Manager::Load_EngineShader(const _tchar* sKey, const _tchar* sShaderFilePath, const D3D_SHADER_MACRO* In_Defines)
{
	ComPtr<ID3DX11Effect> pNewEffect;

	_uint		iHLSLFlag = 0;

#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL1;
	HRESULT hr = D3DX11CompileEffectFromFile(sShaderFilePath, In_Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, DEVICE, pNewEffect.GetAddressOf(), nullptr);

#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
	HRESULT hr = D3DX11CompileEffectFromFile(sShaderFilePath, In_Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, DEVICE, pNewEffect.GetAddressOf(), /*&pError*/nullptr);
#endif

	if (SUCCEEDED(hr))
	{
		_hashcode KeyToHashcode = hash<tstring>()(sKey);

		m_pShaderEffect[KeyToHashcode] = pNewEffect;
	}

	return hr;
}

HRESULT CResource_Manager::Load_Shader_Internal(const _tchar* sKey, const _tchar* sShaderFilePath, ID3DBlob** ppError)
{
	/*ID3DX11Effect* pEffect = Get_ShaderEffect(sKey);

	if (pEffect)
	{
		DEBUG_ASSERT;
		return E_FAIL;
	}*/

	ComPtr<ID3DX11Effect> pNewEffect;

	_uint		iHLSLFlag = 0;

	//ComPtr<ID3DX11Effect> pEffect;


#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL1;
	HRESULT hr = D3DX11CompileEffectFromFile(sShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, DEVICE, pNewEffect.GetAddressOf(), ppError);

#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
	HRESULT hr = D3DX11CompileEffectFromFile(sShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, DEVICE, pNewEffect.GetAddressOf(), /*&pError*/nullptr);
#endif

	

	if (SUCCEEDED(hr))
	{
		_hashcode KeyToHashcode = hash<tstring>()(sKey);

		unique_lock<::mutex> lock(Load_Shader_Mutex);
		m_pShaderEffect[KeyToHashcode] = pNewEffect;
		lock.unlock();
	}

	return hr;
}


HRESULT CResource_Manager::ReLoad_AllShader(list<pair<_bool, string>>& Out_CompileMessage)
{
	Update_ChangedShaderFile(Out_CompileMessage);

	GET_SINGLE(CObject_Manager)->OnEngineEventMessage(ENGINE_EVENT_TYPE::ON_SHADER_UPDATE);
	GET_SINGLE(CRender_Manager)->OnEngineEventMessage(ENGINE_EVENT_TYPE::ON_SHADER_UPDATE);


	return S_OK;
}

ID3DX11Effect* CResource_Manager::Get_ShaderEffect(const _tchar* sKey)
{
	_hashcode KeyToHashcode = hash<tstring>()(sKey);

	auto	iter = m_pShaderEffect.find(KeyToHashcode);//find_if(m_pShaderEffect.begin(), m_pShaderEffect.end(), CTag_Finder(sKey));

	if(m_pShaderEffect.end() == iter)
		return nullptr;

	if (nullptr == iter->second)
	{
		return nullptr;
	}

	return (*iter).second.Get();
}

void CResource_Manager::Update_ChangedShaderFile(list<pair<_bool, string>>& Out_CompileMessage)
{
	uintmax_t iNewFileSize;

	ID3DBlob* pError = nullptr;
	HRESULT hr = S_OK;
	string szOut_Message;

	for (auto& ShaderFile : m_ShaderFilePaths)
	{
		iNewFileSize = filesystem::file_size(filesystem::path(ShaderFile.second.szFilePath));

		if (iNewFileSize != ShaderFile.second.iFileSize)
		{
			auto Shader_Iter = m_pShaderEffect.find(ShaderFile.first);
			ComPtr<ID3DX11Effect> pPreShader = Shader_Iter->second;
			//Shader_Iter->second.Reset();
			//m_pShaderEffect.erase(Shader_Iter);

			pError = nullptr;
			hr = Load_Shader_Internal(ShaderFile.second.szKey.c_str(), ShaderFile.second.szFilePath.c_str(), &pError);

			if (SUCCEEDED(hr))
			{
				pPreShader.Reset();
			}
		}
		else
		{
			hr = S_OK;
		}

		szOut_Message = filesystem::path(ShaderFile.second.szKey).string() + ": ";

		if (pError != nullptr)
		{
			// 에러라면
			Out_CompileMessage.push_back(make_pair<_bool, string>(false, szOut_Message + (char*)pError->GetBufferPointer()));
		}

		else
		{
			// 정상 컴파일
			//string szHR = std::system_category().message(hr);
			Out_CompileMessage.push_back(make_pair<_bool, string>(true, szOut_Message + "S_OK"));
		}
	}

}



void CResource_Manager::OnLevelExit()
{
	Release_ResourceByMemoryType(MEMORY_TYPE::MEMORY_DYNAMIC);

}

void CResource_Manager::Free()
{
	Release_ResourceByMemoryType(MEMORY_TYPE::MEMORY_DYNAMIC);
	Release_ResourceByMemoryType(MEMORY_TYPE::MEMORY_STATIC);

	for (auto& elem : m_pShaderEffect)
	{
		elem.second.Reset();
	}

	m_pShaderEffect.clear();
}
