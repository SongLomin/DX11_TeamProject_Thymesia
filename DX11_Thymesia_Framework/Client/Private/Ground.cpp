#include "stdafx.h"
#include "Ground.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"
#include "ModelData.h"
#include "MeshData.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"

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
		VTXGROUND_DECLARATION::Element,
		VTXGROUND_DECLARATION::iNumElements
	);

	m_pRendererCom      = Add_Component<CRenderer>();
	m_pVIBufferCom      = Add_Component<CVIBuffer_Ground>();
	m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

	/*if((_uint)LEVEL_EDIT != m_CreatedLevel)
		USE_START(CGround);*/

	return S_OK;
}

HRESULT CGround::Start()
{
	__super::Start();

	m_pPhysXColliderCom.lock()->Init_MeshCollider(m_pGroundMeshData);

	PHYSXCOLLIDERDESC ColliderDesc;
	Preset::PhysXColliderDesc::GroundSetting(ColliderDesc, false);

	m_pPhysXColliderCom.lock()->CreatePhysXActor(ColliderDesc);
	m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();
	m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

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

HRESULT CGround::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom.lock()->Begin(m_iShaderPath, pDeviceContext);
	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return S_OK;
}

void CGround::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	if (In_Json.find("TextureInfo") != In_Json.end())
	{
		json TexInfo = In_Json["TextureInfo"];

		for (auto& iter : TexInfo.items())
		{
			string szkey	= iter.key();
			json Textures	= iter.value();

			TEXTURES_INFO Desc;

			for (auto& iter_data : Textures.items())
			{
				string szDatakey = iter_data.key();

				if ("Diff" == szDatakey)
				{
					string szTextureName = iter_data.value();

					Desc.pDiffTex = Add_Component<CTexture>();
					Desc.pDiffTex.lock()->Use_Texture(szTextureName.c_str());
				}

				if ("Norm" == szDatakey)
				{
					string szTextureName = iter_data.value();

					Desc.pNormTex = Add_Component<CTexture>();
					Desc.pNormTex.lock()->Use_Texture(szTextureName.c_str());
				}

				if ("ORM" == szDatakey)
				{
					string szTextureName = iter_data.value();

					Desc.pORMTex = Add_Component<CTexture>();
					Desc.pORMTex.lock()->Use_Texture(szTextureName.c_str());
				}

				if ("Density" == szDatakey)
				{
					Desc.fDensity = iter_data.value();
				}
			}

			if (Desc.pDiffTex.lock() && Desc.pNormTex.lock())
				m_pTextureDescs.emplace(szkey, Desc);
		}
	}

	if (In_Json.find("g_FilterTexture") != In_Json.end())
	{
		string szTextureName = In_Json["g_FilterTexture"];

		m_pFilterTextureCom = Add_Component<CTexture>();
		m_pFilterTextureCom.lock()->Use_Texture(szTextureName.c_str());
	}

	if (In_Json.find("VIBufferCom") != In_Json.end())
	{
		m_szModelName = In_Json["VIBufferCom"];

		shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey(m_szModelName.c_str());

		if (!pModelData.get())
		{
			MSG_BOX("Err : CGround::Load_FromJson(...) <Obj Make By [An Seubg Han]>");
			return;
		}

		m_pGroundMeshData = pModelData.get()->Mesh_Datas[0];

		m_pVIBufferCom.lock()->Init_Mesh(m_pGroundMeshData);
	}

	if (In_Json.find("ShaderPass") != In_Json.end())
	{
		m_iShaderPath = In_Json["ShaderPass"];
	}
}

HRESULT CGround::SetUp_ShaderResource()
{
	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
		return E_FAIL;

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	if (2 == m_iShaderPath)
		return S_OK;
	
	for (auto& iter : m_pTextureDescs)
	{
		string szDiffTextureName = iter.first + "_Diff";
		string szNormTextureName = iter.first + "_Norm";
		string szORMTextureName  = iter.first + "_ORM";
		string szDensityName     = "g_f" + iter.first.substr(string("g_Texture").length() + 1) + "_Density";

		if (FAILED(iter.second.pDiffTex.lock()->Set_ShaderResourceView(m_pShaderCom, szDiffTextureName.c_str(), 0)))
			return E_FAIL;

		if (FAILED(iter.second.pNormTex.lock()->Set_ShaderResourceView(m_pShaderCom, szNormTextureName.c_str(), 0)))
			return E_FAIL;

		if (FAILED(iter.second.pORMTex.lock()->Set_ShaderResourceView(m_pShaderCom, szORMTextureName.c_str(), 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom.lock()->Set_RawValue(szDensityName.c_str(), &iter.second.fDensity, sizeof(_float))))
			return E_FAIL;
	}

	m_pFilterTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_FilterTexture", 0);

	return S_OK;
}

void CGround::Free()
{
}