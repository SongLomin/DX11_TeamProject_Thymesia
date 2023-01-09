#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CShader;
struct MODEL_DATA;

class CResource_Manager :
	public CBase
{
	DECLARE_SINGLETON(CResource_Manager)

private: /* For. Captured Resources */
	struct TEXTURE_DATA
	{
		string		szFilePath;
		_int		MipMapLevels;
	};

	struct SHADER_FILEPATH
	{
		tstring		szKey;
		tstring		szFilePath;
		uintmax_t	iFileSize;
	};

public: /* For Texture */
	HRESULT Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC);
	HRESULT Generate_MipMap(const _tchar* pTextureFilePath, const _int In_iMipMapLevels = 8);
	vector<ComPtr<ID3D11ShaderResourceView>> Get_TexturesFromKey(const _char* _Str_Key, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	

private:
	unordered_map<_hashcode, vector<ComPtr<ID3D11ShaderResourceView>>> m_SRVs[(_uint)MEMORY_TYPE::MEMORY_END];
	unordered_map<string, TEXTURE_DATA> m_TextureFilePaths[(_uint)MEMORY_TYPE::MEMORY_END];

public: /* For Model */
	HRESULT Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC, _bool bAnimZero = false);
	shared_ptr<MODEL_DATA> Get_ModelFromKey(const _char* _sKey, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	vector<const string*> Get_AllModelKeys();
	vector<const string*> Get_AllNoneAnimModelKeys();
	vector<const string*> Get_AllAnimModelKeys();

private:
	unordered_map<string, shared_ptr<MODEL_DATA>>	m_pScenes[(_uint)MEMORY_TYPE::MEMORY_END];

public: /* For Shader */
	HRESULT Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath);
	HRESULT Load_EngineShader(const _tchar* sKey, const _tchar* sShaderFilePath, const D3D_SHADER_MACRO* In_Defines);
	HRESULT ReLoad_AllShader(list<pair<_bool, string>>& Out_CompileMessage);
	ID3DX11Effect* Get_ShaderEffect(const _tchar* sKey);
	void	Update_ChangedShaderFile(list<pair<_bool, string>>& Out_CompileMessage);
	//HRESULT SetUp_AllShaderRawValue();

private:
	HRESULT Load_Shader_Internal(const _tchar* sKey, const _tchar* sShaderFilePath, ID3DBlob** ppError = nullptr);
	

private:
	map<_hashcode, ComPtr<ID3DX11Effect>>			m_pShaderEffect;
	map<_hashcode, SHADER_FILEPATH>					m_ShaderFilePaths;
	

//public: /* For Navigation Mesh */
//	HRESULT Load_NaviMesh(const _char* sKey, const _char* sShaderFilePath);
//	ID3DX11Effect* Get_NaviMesh(const _char* sKey);
//
//private:
//	unordered_map<string, >

public: /* For. Captured Resources */
	HRESULT Release_ResourceByMemoryType(MEMORY_TYPE _eMemType);

	void Write_JsonUsingResource(const char* In_szFilePath);
	void Load_ResourcesFromJson(const char* In_szFilePath);

private:
	void Load_ModelResourcesFromJson(json& In_ModelJson, const MEMORY_TYPE In_eMemoryType);
	void Load_TextureResourcesFromJson(json& In_TextureJson, const MEMORY_TYPE In_eMemoryType);

private:
	json m_UsingResourceJson;


	//map<const _char*, const aiScene*>	m_pScenes[(_uint)MEMORY_TYPE::MEMORY_END];

public:
	virtual void OnLevelExit() override;

private:
	void Free();
};

END