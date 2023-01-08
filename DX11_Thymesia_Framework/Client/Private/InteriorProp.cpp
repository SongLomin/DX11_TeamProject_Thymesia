#include "stdafx.h"
#include "InteriorProp.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameManager.h"
#include "Texture.h"
#include "VIBuffer_Ground.h"

#include "SMath.h"

GAMECLASS_C(CInteriorProp);
CLONE_C(CInteriorProp, CGameObject);

HRESULT CInteriorProp::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteriorProp::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom   = Add_Component<CShader>();
    m_pRendererCom = Add_Component<CRenderer>();
	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxGround"),
		VTXGROUND_DECLARATION::Element,
		VTXGROUND_DECLARATION::iNumElements
	);

	shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey("DefaultGround");
	m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0], D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

    return S_OK;
}

HRESULT CInteriorProp::Start()
{
    return S_OK;
}

void CInteriorProp::Tick(_float fTimeDelta)
{
	Edit_Props();

    __super::Tick(fTimeDelta);
}

void CInteriorProp::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteriorProp::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(SetUp_ShaderResource()))
		DEBUG_ASSERT;

	m_pShaderCom.lock()->Begin(4, pDeviceContext);
	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return __super::Render(pDeviceContext);
}

HRESULT CInteriorProp::SetUp_ShaderResource()
{
	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}

void CInteriorProp::Edit_Props()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return;

	if (!KEY_INPUT(KEY::CTRL, KEY_STATE::TAP))
		return;

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	_float3		Out = _float3(0.f, 0.f, 0.f);

	if (m_pVIBufferCom.lock()->Compute_MousePos
	(
		MouseRayInWorldSpace,
		m_pTransformCom.lock()->Get_WorldMatrix(),
		&Out
	))
	{
		// »ý¼º


	}
}


void CInteriorProp::Free()
{
	__super::Free();
}
