#include "Texture.h"
#include "GameInstance.h"
#include "Shader.h"

GAMECLASS_C(CTexture)
CLONE_C(CTexture, CComponent)

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{

}

string CTexture::Get_TextureKey() const
{
	return m_szTextureKey;
}

HRESULT CTexture::Set_ShaderResourceView(weak_ptr<CShader> pShader, const char* pConstantName, _uint iIndex)
{
	if (iIndex >= m_SRVs.size())
		return E_FAIL;

	return pShader.lock()->Set_ShaderResourceView(pConstantName, m_SRVs[iIndex]);
}

HRESULT CTexture::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{

	return S_OK;
}

void CTexture::Start()
{
}

HRESULT CTexture::Use_Texture(const _char* pTextureKey)
{
	m_SRVs = GAMEINSTANCE->Get_TexturesFromKey(pTextureKey);
	m_szTextureKey = pTextureKey;

	//텍스쳐 불러오기 실패
	if (m_SRVs.empty())
	{
#ifdef _DEBUG
		//wcout << "Fail to Use Texture: " << pTextureKey << endl;

#endif // _DEBUG
		//assert(false);
	}

	return S_OK;



//	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
//
//	for (_uint i = 0; i < 512; ++i)
//	{
//		ComPtr<ID3D11ShaderResourceView> pSRV;
//
//		wsprintf(szTextureFilePath, pTextureKey, i);
//
//		_tchar			szExt[MAX_PATH] = TEXT("");
//
//		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);
//
//		HRESULT		hr = 0;
//
//
//		if (!lstrcmp(szExt, TEXT(".dds")))
//			hr = CreateDDSTextureFromFile(DEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());
//
//		else
//			hr = CreateWICTextureFromFile(DEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());
//
//		if (FAILED(hr))
//			return S_OK;
//
//		m_SRVs.push_back(pSRV);
//
//	}
//#ifdef _DEBUG
//	//텍스쳐를 512까지 읽었음.
//	assert(false);
//#endif
//	return E_FAIL;
}

void CTexture::Write_Json(json& Out_Json)
{
	



}

void CTexture::Load_FromJson(const json& In_Json)
{
}


void CTexture::Free()
{
	__super::Free();

	m_SRVs.clear();
}
