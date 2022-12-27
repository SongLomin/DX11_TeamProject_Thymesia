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
	HRESULT Set_Matrix(const char* pConstantName, _fmatrix In_Matrix);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView> pSRV);
	HRESULT Begin(_uint iPassIndex, ID3D11DeviceContext* pDeviceContext);

	HRESULT Set_ShaderInfo(const _tchar* pShaderKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	HRESULT Set_ShaderInfo_Internal();

public:
	virtual void OnDestroy() override;

private:
	virtual void OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent) override;

private:
	ID3DX11Effect*					m_pEffect = nullptr;

	vector<PASSDESC>				m_Passes;
	typedef vector<PASSDESC>		PASSES;

	tstring							m_szShaderKey;
	const D3D11_INPUT_ELEMENT_DESC*	m_InputDesc;
	_uint							m_iNumElement;
	
	_bool							m_bInit = false;

public:
	/*static CShader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElement);
	virtual CComponent* Clone(void* pArg) override;*/
	void Free();
};

END