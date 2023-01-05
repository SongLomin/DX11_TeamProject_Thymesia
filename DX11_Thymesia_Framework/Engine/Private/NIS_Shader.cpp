#include "NIS_Shader.h"
#include "GameInstance.h"
#include "Resource_Manager.h"
#include "DXUtilities.h"

GAMECLASS_C(CNIS_Shader)
CLONE_C(CNIS_Shader, CComponent)


HRESULT CNIS_Shader::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CNIS_Shader::Initialize(void* pArg)
{
	_bool isUpscaling = false;
	// Note: NISOptimizer is optional and these values can be cached offline
	NISOptimizer opt(isUpscaling, NISGPUArchitecture::NVIDIA_Generic);
	m_blockWidth = opt.GetOptimalBlockWidth();
	m_blockHeight = opt.GetOptimalBlockHeight();
	uint32_t threadGroupSize = opt.GetOptimalThreadGroupSize();
	
	D3D_SHADER_MACRO defines[6];

	m_definesVector.push_back({ "NIS_SCALER", to_string(isUpscaling) });
	m_definesVector.push_back({ "NIS_HDR_MODE", to_string(uint32_t(NISHDRMode::None)) });
	m_definesVector.push_back({ "NIS_BLOCK_WIDTH", to_string(m_blockWidth) });
	m_definesVector.push_back({ "NIS_BLOCK_HEIGHT", to_string(m_blockHeight) });
	m_definesVector.push_back({ "NIS_THREAD_GROUP_SIZE", to_string(threadGroupSize) });

	for (_size_t i = 0; i < m_definesVector.size(); ++i)
	{
		defines[i].Name = m_definesVector[i].first.c_str();
		defines[i].Definition = m_definesVector[i].second.c_str();
	}

	defines[m_definesVector.size()].Name = NULL;
	defines[m_definesVector.size()].Definition = NULL;


	DX::IncludeHeader includeHeader({ "../Bin/Shaderfiles/" });

	DX::CompileComputeShader(DEVICE,
		TEXT("../Bin/Shaderfiles/NIS_Main.hlsl"),
		"main",
		&m_computeShader,
		defines,
		&includeHeader);

	//GET_SINGLE(CResource_Manager)->Load_EngineShader(TEXT("NIS_Main"), TEXT("../Bin/Shaderfiles/NIS_Main.hlsl"), defines);
	

	const int rowPitch = kFilterSize * 4;
	const int imageSize = rowPitch * kPhaseCount;
	
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(DEVICE->CreateSamplerState(&samplerDesc, m_LinearClampSampler.GetAddressOf()));

	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = sizeof(NISConfig);
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA srData;
	srData.pSysMem = &m_config;
	DX::ThrowIfFailed(DEVICE->CreateBuffer(&bDesc, &srData, &m_csBuffer));
	
	//m_pInclude = Open_IncludeHeader(D3D_INCLUDE_TYPE::D3D_INCLUDE_LOCAL, "../Bin/Shaderfiles/NIS_Scaler.h", NULL);

	/*pInclude->Open(D3D_INCLUDE_TYPE::D3D_INCLUDE_LOCAL, );

	HRESULT hr = D3DCompileFromFile(TEXT("../Bin/Shaderfiles/NIS_Main.hlsl"), defines, pInclude.Get(), pEntryPoint, pTarget, 0, 0, &csBlob, &cdErrorBlob);
	if (FAILED(hr)) {
		if (cdErrorBlob) {
			OutputDebugStringA((char*)cdErrorBlob->GetBufferPointer());
		}
	}
	DX::ThrowIfFailed(device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, csShader));*/

	return S_OK;
}

void CNIS_Shader::Start()
{

}

void CNIS_Shader::Update(_float sharpness, uint32_t inputWidth, uint32_t inputHeight)
{
	NVSharpenUpdateConfig(m_config, sharpness,
		0, 0, inputWidth, inputHeight, inputWidth, inputHeight,
		0, 0, NISHDRMode::None);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DEVICECONTEXT->Map(m_csBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	uint8_t* mappData = (uint8_t*)mappedResource.pData;
	memcpy(mappData, &m_config, sizeof(NISConfig));
	DEVICECONTEXT->Unmap(m_csBuffer.Get(), 0);


	//m_deviceResources.updateConstBuffer(&m_config, sizeof(NISConfig), m_csBuffer.Get());

	m_outputWidth = inputWidth;
	m_outputHeight = inputHeight;
}

void CNIS_Shader::Update(_float sharpness)
{
	NVSharpenUpdateConfig(m_config, sharpness,
		0, 0, m_outputWidth, m_outputHeight, m_outputWidth, m_outputHeight,
		0, 0, NISHDRMode::None);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DEVICECONTEXT->Map(m_csBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	uint8_t* mappData = (uint8_t*)mappedResource.pData;
	memcpy(mappData, &m_config, sizeof(NISConfig));
	DEVICECONTEXT->Unmap(m_csBuffer.Get(), 0);
}

void CNIS_Shader::Dispatch(ID3D11ShaderResourceView* const* input, ID3D11UnorderedAccessView* const* output)
{
	ID3D11DeviceContext* context = DEVICECONTEXT;
	context->CSSetShaderResources(0, 1, input);
	context->CSSetUnorderedAccessViews(0, 1, output, nullptr);
	context->CSSetSamplers(0, 1, m_LinearClampSampler.GetAddressOf());
	context->CSSetConstantBuffers(0, 1, m_csBuffer.GetAddressOf());
	context->CSSetShader(m_computeShader.Get(), nullptr, 0);
	context->Dispatch(UINT(std::ceil(m_outputWidth / float(m_blockWidth))), UINT(std::ceil(m_outputHeight / float(m_blockHeight))), 1);


}

//HRESULT CNIS_Shader::Open_IncludeHeader(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData)
//{// Open the file and read its contents into a buffer
//	std::ifstream file(pFileName, std::ios::binary);
//	LPCVOID* ppData;
//	UINT* pBytes;
//
//	if (!file)
//	{
//		return E_FAIL;
//	}
//
//	file.seekg(0, std::ios::end);
//	size_t size = (size_t)file.tellg();
//	file.seekg(0, std::ios::beg);
//
//	char* pData = DBG_NEW _char[size + 1];
//	file.read(pData, size);
//	pData[size] = '\0';
//
//	*ppData = pData;
//	*pBytes = (UINT)size;
//
//	ID3DInclude* pInclude = nullptr;
//
//	pInclude->Open(IncludeType, pFileName, pParentData, ppData, pBytes);
//
//
//	return S_OK;
//}


void CNIS_Shader::Free()
{

}
