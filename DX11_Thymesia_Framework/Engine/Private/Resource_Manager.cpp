#include "Resource_Manager.h"
#include "GameInstance.h"
#include "ModelData.h"

IMPLEMENT_SINGLETON(CResource_Manager)

HRESULT CResource_Manager::Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType)
{
	_tchar szTextureFilePath[MAX_PATH] = TEXT("");

	_hashcode KeyToHash = hash<string>()(_strKey);

	auto iter = m_SRVs[(_uint)eMemType].find(KeyToHash);
	//auto iter = find_if(m_SRVs[(_uint)eMemType].begin(), m_SRVs[(_uint)eMemType].end(), CTag_Finder_c_str(_strKey));

	

	if (m_SRVs[(_uint)eMemType].end() != iter)
		return S_OK;

	

	HRESULT hr(0);
	_tchar szExt[MAX_PATH] = TEXT("");
	_bool bCreated = false;

	for (_uint i(0); i < 1024; ++i)
	{
		ComPtr<ID3D11ShaderResourceView> pSRV;
		if (lstrcmpW(szTextureFilePath, pTextureFilePath) == 0)
		{
			m_TextureFilePaths[(_uint)eMemType][_strKey] = std::filesystem::path(pTextureFilePath).string();
			return S_OK;
		}

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		ZeroMemory(szExt, sizeof(_tchar) * MAX_PATH);
		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		if (!lstrcmp(szExt, TEXT(".dds"))) hr = CreateDDSTextureFromFile(DEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());
		else hr = CreateWICTextureFromFile(DEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());

		if (FAILED(hr))
		{
			if (bCreated)
			{
				m_TextureFilePaths[(_uint)eMemType][_strKey] = std::filesystem::path(pTextureFilePath).string();
				return S_OK;
			}
			else
				return E_FAIL;
		}

		m_SRVs[(_uint)eMemType][KeyToHash].emplace_back(pSRV);
		bCreated = true;
	}
#ifdef _DEBUG
	//텍스쳐를 512까지 읽었음.
	// assert(false);
#endif
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
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_STATIC]["Textures"][_Str_Key] = m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_STATIC][_Str_Key];
			return (*iter).second;
		}
		
		iter = m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].find(KeyToHash);
		//iter = find_if(m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].begin(), m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].end(), CTag_Finder_c_str(_Str_Key));

		if (m_SRVs[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC].end() != iter)
		{
			m_UsingResourceJson[(_int)MEMORY_TYPE::MEMORY_DYNAMIC]["Textures"][_Str_Key] = m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_DYNAMIC][_Str_Key];
			return (*iter).second;
		}
	}
	else
	{
		auto	iter = m_SRVs[(_uint)_eType].find(KeyToHash);

		if (m_SRVs[(_uint)_eType].end() != iter)
		{
			m_UsingResourceJson[(_int)_eType]["Textures"][_Str_Key] = m_TextureFilePaths[(_int)_eType][_Str_Key];
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
	}

	list<future<void>>	Threads;

	for (_int i = 0; i < 2; ++i)
	{
		Threads.push_back(async(launch::async,
			bind(&CResource_Manager::Load_ModelResourcesFromJson, this, placeholders::_1, placeholders::_2), 
			In_Json[i]["Model"], (MEMORY_TYPE)i));

		Threads.push_back(async(launch::async,
			bind(&CResource_Manager::Load_TextureResourcesFromJson, this, placeholders::_1, placeholders::_2),
			In_Json[i]["Textures"], (MEMORY_TYPE)i));
	}

	for (auto& elem : Threads)
	{
		elem.get();
	}
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

	for (auto json_iter : In_TextureJson.items())
	{
		FilePath = json_iter.value();

		szTexturePath = std::filesystem::path(FilePath).wstring();

		Load_Textures(json_iter.key().c_str(), szTexturePath.c_str(), In_eMemoryType);
	}
}

HRESULT CResource_Manager::Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType, _bool bAnimZero)
{
	

	shared_ptr<MODEL_DATA> TempModel = make_shared<MODEL_DATA>();

	char			szFullPath[MAX_PATH] = "";
	strcpy_s(szFullPath, sModelFilePath);

	auto iter = m_pScenes[(_uint)eMemType].find(sKey);
	//기존에 이미 같은 키로 등록된 모델이 있음.
	if (m_pScenes[(_uint)eMemType].end() != iter)
		return E_FAIL;
	
	if (FAILED(TempModel->Make_ModelData(szFullPath, eModelType, In_TransformMatrix, bAnimZero)))
	{
		return E_FAIL;
	}

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

vector<string> CResource_Manager::Get_AllModelKeys()
{
	vector<string> ResultKeys;

	for (auto& elem_List : m_pScenes)
	{
		for (auto& elem : elem_List)
		{
			ResultKeys.push_back(elem.first);
		}
	}

	return ResultKeys;
}

vector<string> CResource_Manager::Get_AllNoneAnimModelKeys()
{
	vector<string> ResultKeys;

	for (auto& elem_List : m_pScenes)
	{
		for (auto& elem : elem_List)
		{
			if (MODEL_TYPE::NONANIM == elem.second->eModelType)
				ResultKeys.push_back(elem.first);
		}
	}

	return ResultKeys;
}

vector<string> CResource_Manager::Get_AllAnimModelKeys()
{
	vector<string> ResultKeys;

	for (auto& elem_List : m_pScenes)
	{
		for (auto& elem : elem_List)
		{
			if(MODEL_TYPE::ANIM == elem.second->eModelType)
				ResultKeys.push_back(elem.first);
		}
	}

	return ResultKeys;
}

HRESULT CResource_Manager::Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath)
{
	ID3DX11Effect* pEffect = Get_ShaderEffect(sKey);

	if (pEffect)
	{
		DEBUG_ASSERT;
		return E_FAIL;
	}

	ComPtr<ID3DX11Effect> pNewEffect;

	_uint		iHLSLFlag = 0;

	//ComPtr<ID3DX11Effect> pEffect;

#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
	D3DX11CompileEffectFromFile(sShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, DEVICE, pNewEffect.GetAddressOf(), /*&pError*/nullptr);

	m_pShaderEffect.emplace(sKey, pNewEffect);

	return S_OK;
}

ID3DX11Effect* CResource_Manager::Get_ShaderEffect(const _tchar* sKey)
{
	auto	iter = find_if(m_pShaderEffect.begin(), m_pShaderEffect.end(), CTag_Finder(sKey));

	if(m_pShaderEffect.end() == iter)
		return nullptr;

	return (*iter).second.Get();
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
