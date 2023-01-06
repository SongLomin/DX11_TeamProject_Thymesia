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
}

void CWater::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
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