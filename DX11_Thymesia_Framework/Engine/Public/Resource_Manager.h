#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
struct MODEL_DATA;

class CResource_Manager :
	public CBase
{
	DECLARE_SINGLETON(CResource_Manager)

public: /* For Texture */
	HRESULT Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC);
	//vector*를 반환하지 않는 이유: 셰이더리소스뷰의 레퍼런스 카운트를 올리기 위해서 쓰지 않는다.
	vector<ComPtr<ID3D11ShaderResourceView>> Get_TexturesFromKey(const _char* _Str_Key, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	

private:
	unordered_map<string, vector<ComPtr<ID3D11ShaderResourceView>>> m_SRVs[(_uint)MEMORY_TYPE::MEMORY_END];

public: /* For Model */
	HRESULT Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC, _bool bAnimZero = false);
	shared_ptr<MODEL_DATA> Get_ModelFromKey(const _char* _sKey, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	vector<string> Get_AllModelKeys();
	vector<string> Get_AllAnimModelKeys();

private:
	unordered_map<string, shared_ptr<MODEL_DATA>>	m_pScenes[(_uint)MEMORY_TYPE::MEMORY_END];

public: /* For Shader */
	HRESULT Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath);
	ID3DX11Effect* Get_ShaderEffect(const _tchar* sKey);
	//HRESULT SetUp_AllShaderRawValue();

private:
	unordered_map<tstring, ComPtr<ID3DX11Effect>> m_pShaderEffect;

//public: /* For Navigation Mesh */
//	HRESULT Load_NaviMesh(const _char* sKey, const _char* sShaderFilePath);
//	ID3DX11Effect* Get_NaviMesh(const _char* sKey);
//
//private:
//	unordered_map<string, >

public:
	HRESULT Release_ResourceByMemoryType(MEMORY_TYPE _eMemType);


	//map<const _char*, const aiScene*>	m_pScenes[(_uint)MEMORY_TYPE::MEMORY_END];

public:
	virtual void OnLevelExit() override;

private:
	virtual void Free() override;
};

END