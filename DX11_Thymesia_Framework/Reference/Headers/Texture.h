#pragma once
#include "Component.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CTexture final : public CComponent
{
	GAMECLASS_H(CTexture)
	CLONE_H(CTexture, CComponent)

public:
	CTexture(const CTexture& rhs);

public:
	string Get_TextureKey() const;

public:
	HRESULT Set_ShaderResourceView(weak_ptr<CShader> pShader, const char* pConstantName, _uint iIndex = 0);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

public:
	HRESULT Use_Texture(const _char* pTextureKey);

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	vector<ComPtr<ID3D11ShaderResourceView>>			m_SRVs;
	string												m_szTextureKey;

public:
	void Free();
};

END