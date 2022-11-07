#pragma once
#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
	GAMECLASS_H(CShader)
	CLONE_H(CShader, CComponent)

public:
	typedef struct tagPasses
	{
		ID3DX11EffectPass* pPass;
		ID3D11InputLayout* pInputLayout;
	}PASSDESC;

public:
	CShader(const CShader& rhs);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

	virtual void Tick(_float fTimeDelta) override;

public:
	HRESULT Set_RawValue(const char* pConstantName, void* pData, _uint iDataSize);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView> pSRV);
	HRESULT Begin(_uint iPassIndex);

	HRESULT Set_ShaderInfo(const _tchar* pShaderKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);

public:
	virtual void OnDestroy() override;

private:
	ID3DX11Effect* m_pEffect;

	vector<PASSDESC>				m_Passes;
	typedef vector<PASSDESC>		PASSES;

public:
	/*static CShader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElement);
	virtual CComponent* Clone(void* pArg) override;*/
	virtual void Free() override;
};

END