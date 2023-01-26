#include "stdafx.h"

#include "Water.h"

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"
#include "ModelData.h"
#include "MeshData.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"
#include "GameManager.h"

GAMECLASS_C(CWater)
CLONE_C(CWater, CGameObject)

HRESULT CWater::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxGround"),
		VTXGROUND_DECLARATION::Element,
		VTXGROUND_DECLARATION::iNumElements
	);

	m_pRendererCom     = Add_Component<CRenderer>();
	m_pVIBufferCom     = Add_Component<CVIBuffer_Ground>();
	m_pNoiseTextureCom = Add_Component<CTexture>();

	m_pNoiseTextureCom.lock()->Use_Texture("UVNoise");

	shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey("DefaultGround");
	m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0], D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	m_WaterWaveDescs.resize(128, {});
	

	GET_SINGLE(CGameManager)->Register_Water(Weak_StaticCast<CWater>(m_this));

	return S_OK;
}

HRESULT CWater::Start()
{
	__super::Start();

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

	return S_OK;
}

void CWater::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_vNoiseUV.x += fTimeDelta;
	sort(m_WaterWaveDescs.begin(), m_WaterWaveDescs.end(), [](const WATERWAVE_DESC& left, const WATERWAVE_DESC& right) {
			return left.fVibrationScale > right.fVibrationScale;
			});

	m_iDescCount = 0;

	m_fTimeAcc += fTimeDelta;
	while (m_fTimeAcc >= m_fTimeInterval)
	{
		++m_iPowCount;
		m_fTimeAcc -= m_fTimeInterval;
	}

	for (_uint i = 0; i < m_WaterWaveDescs.size(); ++i)
	{
 		m_WaterWaveDescs[i].fVibrationScale *= pow(0.9f, m_iPowCount);
		m_WaterWaveDescs[i].fTimeAcc += fTimeDelta;

		if (m_WaterWaveDescs[i].fVibrationScale > 0.001f)
		{
			++m_iDescCount;
		}
			
	}
	m_iPowCount = 0;
	
}

void CWater::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_WATER, Cast<CGameObject>(m_this));
}

HRESULT CWater::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	if (FAILED(SetUp_ShaderResource()))
		DEBUG_ASSERT;

	m_pShaderCom.lock()->Begin(4, pDeviceContext);
	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return S_OK;
}

void CWater::Add_WaterWave(const WATERWAVE_DESC& In_WaterDesc)
{
	m_iDescCount = 0;
	
	for (_uint i = 0; i < m_WaterWaveDescs.size(); ++i)
	{
		if (m_WaterWaveDescs[i].fVibrationScale < 0.001f)
		{
			m_WaterWaveDescs[i] = In_WaterDesc;
			m_iDescCount = i;
			return;
		}
	}
	
}

void CWater::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);
}

HRESULT CWater::SetUp_ShaderResource()
{
	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	//_matrix WorldMatrixInv = XMMatrixTranspose(XMMatrixInverse(nullptr, m_pTransformCom.lock()->Get_WorldMatrix()));

	//if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrixInv", &WorldMatrixInv, sizeof(_float4x4))))
	//	return E_FAIL;

	//D3D11_BUFFER_DESC cbDesc;
	//cbDesc.ByteWidth = sizeof(WATERWAVE_DESC);
	//cbDesc.Usage = D3D11_USAGE_DEFAULT;
	//cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbDesc.CPUAccessFlags = 0;
	//cbDesc.MiscFlags = 0;
	//cbDesc.StructureByteStride = 0;

	//ID3D11Buffer* pCB = NULL;
	//HRESULT hr = DEVICE->CreateBuffer(&cbDesc, NULL, &pCB);

	//// Fill the constant buffer with data.
	//D3D11_MAPPED_SUBRESOURCE MappedResource;
	//DEVICECONTEXT->Map(pCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	//WATERWAVE_DESC* pCBuffer = (WATERWAVE_DESC*)MappedResource.pData;

	//// Set the values for the element at index 5 in the structure array.
	//pCBuffer->g_array[5].position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//pCBuffer->g_array[5].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	//DEVICECONTEXT->Unmap(pCB, 0);

	//// Bind the constant buffer to the pipeline.
	//pImmediateContext->VSSetConstantBuffers(0, 1, &pCB);


	// WaterWaveDesc을 배열 크기(최대 128개)만큼 던진다.
	// 배열이 있을때만 던진다.
	if (m_iDescCount > 0)
	{
		// 셰이더 내부의 WATERWAVE_DESC과 프로젝트의 WATERWAVE_DESC의 바이트와 멤버 위치가 같아야됨.
		/*if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WaveDescArray", &m_WaterWaveDescs[0], sizeof(WATERWAVE_DESC) * iWaverWaveCnt)))
		{
			DEBUG_ASSERT;
		}*/

		if (FAILED(m_pShaderCom.lock()->Set_RawValue("WaterWaveDescs", &m_WaterWaveDescs[0], sizeof(WATERWAVE_DESC) * 128)))
		{
			DEBUG_ASSERT;
		}
		
	}

	// 사이즈가 0이더라도, 0이라는 것을 알려준다.
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WaterWaveSize", &m_iDescCount, sizeof(_uint))))
	{
		DEBUG_ASSERT;
	}

	m_pNoiseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DisplacementTexture", 679);

	m_pShaderCom.lock()->Set_RawValue("g_vUVNoise", &m_vNoiseUV, sizeof(_float2));

	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
		DEBUG_ASSERT;

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));


	return S_OK;
}

void CWater::Free()
{
}