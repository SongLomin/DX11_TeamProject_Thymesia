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

	//shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey("MemoryYamYam");

	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	// m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0]);

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
		DEBUG_ASSERT;

	m_pShaderCom.lock()->Begin(0);

	//m_pVIBufferCom.lock()->Render();

	return S_OK;
}

void CGround::Write_Json(json& Out_Json)
{
	Out_Json["TextureName"] = m_szTextureName;
	Out_Json["Pitch"] = m_iPitch;
}

void CGround::Load_FromJson(const json& In_Json)
{
	// Find로 제이슨에 해당하는 키가 있는지 확인
	if (In_Json.find("TextureName") != In_Json.end())
	{
		// 있다면 대입.
		m_szTextureName = In_Json["TextureName"];
	}

	if (In_Json.find("Pitch") != In_Json.end())
	{
		m_iPitch = In_Json["Pitch"];
	}
}

HRESULT CGround::SetUp_ShaderResource()
{
	//CallBack_Bind_SRV(m_pShaderCom, "");

	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		DEBUG_ASSERT;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		DEBUG_ASSERT;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		DEBUG_ASSERT;

	for (auto& iter : m_pTextureCom)
	{
		string szDiffTextureName = iter.first + "_Diff";
		string szNormTextureName = iter.first + "_Norm";
		string szDensityName     = "g_f" + iter.first.substr(string("g_Texture").length() + 1) + "_Density";

		if (FAILED(iter.second.pDiffTex.lock()->Set_ShaderResourceView(m_pShaderCom, szDiffTextureName.c_str(), 0)))
			return E_FAIL;

		if (FAILED(iter.second.pNormTex.lock()->Set_ShaderResourceView(m_pShaderCom, szNormTextureName.c_str(), 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom.lock()->Set_RawValue(szDensityName.c_str(), &iter.second.fDensity, sizeof(_float))))
			return E_FAIL;
	}

	_vector vLightFlag = { 0.f, 0.f, 1.f, 0.f };

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector))))
		DEBUG_ASSERT;

	return S_OK;
}

void CGround::Free()
{
}