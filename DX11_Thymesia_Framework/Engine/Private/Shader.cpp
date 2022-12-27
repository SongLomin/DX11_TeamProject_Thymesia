#include "Shader.h"
#include "GameInstance.h"
#include "GameObject.h"

GAMECLASS_C(CShader)
CLONE_C(CShader, CComponent)

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
{

}

HRESULT CShader::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{

	return S_OK;
}

void CShader::Start()
{
}

void CShader::Tick(_float fTimeDelta)
{

}

HRESULT CShader::Set_RawValue(const char* pConstantName, void* pData, _uint iDataSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iDataSize);
}

HRESULT CShader::Set_Matrix(const char* pConstantName, _fmatrix In_Matrix)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;


	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pVariable)
		return E_FAIL;
	
	return pMatrixVariable->SetMatrix(In_Matrix.r->m128_f32);
}

HRESULT CShader::Begin(_uint iPassIndex, ID3D11DeviceContext* pDeviceContext)
{
	if (!pDeviceContext)
	{
		pDeviceContext = DEVICECONTEXT;
	}

	if (iPassIndex >= m_Passes.size())
		return E_FAIL;
	
	pDeviceContext->IASetInputLayout(m_Passes[iPassIndex].pInputLayout);

	if (FAILED(m_Passes[iPassIndex].pPass->Apply(0, pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Set_ShaderInfo(const _tchar* pShaderKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	m_szShaderKey = pShaderKey;
	m_InputDesc = pElements;
	m_iNumElement = iNumElement;

	m_bInit = true;

	if (FAILED(Set_ShaderInfo_Internal()))
	{
		DEBUG_ASSERT;
	}

	return S_OK;
}

HRESULT CShader::Set_ShaderInfo_Internal()
{
	if (!m_bInit)
	{
		// Call the 'Set_ShaderInfo' function at least once.
		return E_FAIL;
	}
	
	for (auto& elem : m_Passes)
	{
		elem.pInputLayout->Release();
	}

	m_Passes.clear();
	
	/*if (m_pEffect)
	{
		m_pEffect->Release();
	}*/

	m_pEffect = GAMEINSTANCE->Get_ShaderEffect(m_szShaderKey.c_str());

	if (m_pEffect == nullptr)
		DEBUG_ASSERT;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	_uint		iNumPasses = TechniqueDesc.Passes;

	m_Passes.reserve(iNumPasses);

	for (_uint i = 0; i < iNumPasses; ++i)
	{
		PASSDESC			PassDesc;
		ZeroMemory(&PassDesc, sizeof(PASSDESC));

		PassDesc.pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == PassDesc.pPass)
			return E_FAIL;

		D3DX11_PASS_DESC		Pass;
		ZeroMemory(&Pass, sizeof(D3DX11_PASS_DESC));

		PassDesc.pPass->GetDesc(&Pass);

		HRESULT hr = (DEVICE->CreateInputLayout(m_InputDesc, m_iNumElement, Pass.pIAInputSignature, Pass.IAInputSignatureSize, &PassDesc.pInputLayout));
		
		if (FAILED(hr))
		{
			return hr;
		}

		m_Passes.push_back(PassDesc);
	}

	return S_OK;
}


HRESULT CShader::Set_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView> pSRV)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	//m_pEffect->GetV
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pVariable_SRV = pVariable->AsShaderResource();

	return pVariable_SRV->SetResource(pSRV.Get());
}



//CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
//{
//	CShader* pInstance = new CShader(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElement)))
//	{
//		MSG_BOX("Failed to Created : CShader");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CComponent* CShader::Clone(void* pArg)
//{
//	CShader* pInstance = new CShader(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Created : CShader");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

void CShader::OnDestroy()
{
}

void CShader::OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent)
{
	if (In_eEngineEvent == ENGINE_EVENT_TYPE::ON_SHADER_UPDATE)
	{
		Set_ShaderInfo_Internal();
	}
}

void CShader::Free()
{
	for (auto& elem : m_Passes)
	{
		elem.pInputLayout->Release();
	}

	m_Passes.clear();
	m_pEffect = nullptr;
	
}
