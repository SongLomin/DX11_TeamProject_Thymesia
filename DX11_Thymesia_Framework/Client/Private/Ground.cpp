#include "stdafx.h"

#include "Ground.h"

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"
#include "ModelData.h"
#include "MeshData.h"

GAMECLASS_C(CGround)
CLONE_C(CGround, CGameObject)

HRESULT CGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGround::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxGround"),
		VTXNORTEX_DECLARATION::Element,
		VTXNORTEX_DECLARATION::iNumElements
	);

	m_pRendererCom		 = Add_Component<CRenderer>();
	m_pDiff_TextureCom	 = Add_Component<CTexture>();
	m_pNorm_TextureCom	 = Add_Component<CTexture>();

	m_pDiff_TextureCom.lock()->Use_Texture("T_Floor_01a_C.png");
	m_pNorm_TextureCom.lock()->Use_Texture("T_Floor_01a_N.png");

	shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey("MemoryYamYam");

	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	// m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0]);

	Set_OwnerForMyComponents();

	return S_OK;
}

HRESULT CGround::Start()
{
	__super::Start();

	return S_OK;
}

void CGround::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGround::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CGround::Render()
{
	__super::Render();

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom.lock()->Begin(0);

	m_pVIBufferCom.lock()->Render();

	return S_OK;
}

HRESULT CGround::SetUp_ShaderResource()
{
	//CallBack_Bind_SRV(m_pShaderCom, "");

	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	if (m_pDiff_TextureCom.lock())
	{
		if (FAILED(m_pDiff_TextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_SourDiffTexture", 0)))
			return E_FAIL;
	}

	if (m_pNorm_TextureCom.lock())
	{
		if (FAILED(m_pNorm_TextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture", 0)))
			return E_FAIL;
	}

	_vector vLightFlag = { 0.f, 0.f, 1.f, 0.f };

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector))))
		return E_FAIL;

	return S_OK;
}

void CGround::Free()
{
}