#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNIS_Shader : public CComponent
{
	GAMECLASS_H(CNIS_Shader)
	CLONE_H(CNIS_Shader, CComponent)


private:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

public:
	void Update(_float sharpness, uint32_t inputWidth, uint32_t inputHeight);
	void Update(_float sharpness);
	void Dispatch(ID3D11ShaderResourceView* const* input, ID3D11UnorderedAccessView* const* output);

private:
	//HRESULT Open_IncludeHeader(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData);

private:
	::vector<pair<string, string>>		m_definesVector;
	NISConfig                           m_config;
	ComPtr<ID3D11ComputeShader>         m_computeShader;
	ComPtr<ID3D11Buffer>                m_csBuffer;
	ComPtr<ID3D11SamplerState>			m_LinearClampSampler;
	uint32_t                            m_outputWidth;
	uint32_t                            m_outputHeight;
	uint32_t                            m_blockWidth;
	uint32_t                            m_blockHeight;

private:
	void Free();


	

};

END