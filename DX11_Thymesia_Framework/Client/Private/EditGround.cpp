#include "stdafx.h"

#include "EditGround.h"
#include "SMath.h"

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"

GAMECLASS_C(CEditGround)
CLONE_C(CEditGround, CGameObject)

HRESULT CEditGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditGround::Initialize(void* pArg)
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

	Load_AllMeshInfo();

	return S_OK;
}

HRESULT CEditGround::Start()
{
	return S_OK;
}

void CEditGround::Tick(_float fTimeDelta)
{
	if (!m_bCreate)
		return;

	if (m_pVIBufferCom.lock() && (EDIT_MODE::HEIGHT_FLAT == m_eEditMode || EDIT_MODE::HEIGHT_LERP == m_eEditMode))
	{
		if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
			PickingGround();
	}
}

void CEditGround::LateTick(_float fTimeDelta)
{
	if (!m_bCreate)
		return;

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CEditGround::Render()
{
	if (!m_bCreate)
		return S_OK;

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom.lock()->Begin(m_iShaderPass);

	m_pVIBufferCom.lock()->Render();

	return S_OK;
}

HRESULT CEditGround::SetUp_ShaderResource()
{
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

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_fDensity", &m_fDensity, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CEditGround::SetUp_EditMode()
{
	const char* items[] =
	{
		"HEIGHT_FLAT",
		"HEIGHT_LERP",
		"FILLTER",
		"NON"
	};

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::NUM1, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::HEIGHT_FLAT;
		else if (KEY_INPUT(KEY::NUM2, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::HEIGHT_LERP;
		else if (KEY_INPUT(KEY::NUM3, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::FILLTER;
		else if (KEY_INPUT(KEY::NUM4, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::NON;
	}

	if (ImGui::BeginListBox("Editer Type"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = ((_uint)m_eEditMode == n);
			if (ImGui::Selectable(items[n], is_selected))
			{
				m_eEditMode = (EDIT_MODE)n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	ImGui::Separator();
}

void CEditGround::SetUp_Info()
{
	ImGui::Text("Size X, Z, Pitch");
	ImGui::DragFloat3("##Size", &m_vBufferInfo.x, 1.f);
	ImGui::Separator();

	ImGui::Text("Density");
	ImGui::DragFloat("##Density", &m_fDensity, 1.f);
	ImGui::Separator();

	if (ImGui::Button("Create"))
	{
		CreateBuffer();
	}
}

void CEditGround::SetUp_TextureComponent()
{

}

void CEditGround::SetUp_ShaderComponent()
{
	ImGui::Text("Shader Pass");
	ImGui::DragInt("##Pass", &m_iShaderPass, 1.f);
	ImGui::Separator();
}

void CEditGround::SetUp_PinckingInfo()
{
	ImGui::Text("Shader DrawRadious,");
	ImGui::DragFloat("##Radious", &m_fBufferDrawRadious, 1.f);
	ImGui::Separator();

	ImGui::Text("Power");
	ImGui::DragFloat("##Power", &m_fBufferPower, 1.f);
	ImGui::Separator();
}

void CEditGround::SetUp_FillterInfo()
{
}

void CEditGround::CreateBuffer()
{
	Remove_Components<CVIBuffer_Ground>();

	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	m_pVIBufferCom.lock()->Init_Mesh(m_vBufferInfo);

	/*D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width				= m_vBufferInfo.x * m_vBufferInfo.z;
	TextureDesc.Height				= m_vBufferInfo.y * m_vBufferInfo.z;
	TextureDesc.MipLevels			= 1;
	TextureDesc.ArraySize			= 1;
	TextureDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality	= 0;
	TextureDesc.SampleDesc.Count	= 1;

	TextureDesc.Usage				= D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags			= 0;*/

	/*if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D.Get())))
		return;*/


	m_bCreate = true;
}

void CEditGround::PickingGround()
{
	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	if (m_pVIBufferCom.lock())
	{
		_float3		Out = _float3(0.f, 0.f, 0.f);

		if (m_pVIBufferCom.lock()->Compute_MousePos(
				MouseRayInWorldSpace,
				m_pTransformCom.lock()->Get_WorldMatrix(),
				&Out))
		{
			m_pVIBufferCom.lock()->Update
			(
				XMLoadFloat3(&Out),
				m_fBufferDrawRadious,
				m_fBufferPower,
				(_uint)m_eEditMode
			);
		}
	}
}

void CEditGround::Bake_Mesh()
{
	MODEL_DATA tModelData;

	tModelData.eModelType		= MODEL_TYPE::GROUND;
	tModelData.iNumAnimations	= 0;
	tModelData.iNumMaterials	= 0;
	tModelData.iNumMeshs		= 1;
	tModelData.szModelFileName	= m_szMeshName;
	tModelData.szModelFilePath	= "../Bin/GroundInfo/" + m_szMeshName + ".bin";
	XMStoreFloat4x4(&tModelData.TransformMatrix, m_pTransformCom.lock()->Get_WorldMatrix());

	tModelData.RootNode = make_shared<NODE_DATA>();
	tModelData.RootNode->iNumChildren = 0;
	XMStoreFloat4x4(&tModelData.RootNode->TransformationMatrix, XMMatrixIdentity());

	_float3 vVertice[3];

	shared_ptr<MESH_DATA> pMeshData = make_shared<MESH_DATA>();
	pMeshData->eModelType		= MODEL_TYPE::GROUND;
	pMeshData->iMaterialIndex	= 0;
	pMeshData->iNumBones		= 0;
	pMeshData->iNumFaces		= _uint(m_vBufferInfo.x - 1) * _uint(m_vBufferInfo.y - 1) * 2;
	pMeshData->iNumVertices		= m_vBufferInfo.x * m_vBufferInfo.y;

	pMeshData->pGroundVertices = shared_ptr<VTXNORTEX[]>(new VTXNORTEX[pMeshData->iNumVertices]);

	_float3 vNorm = _float3(0.f, 1.f, 0.f);
	for (_uint i = 0; i < pMeshData->iNumVertices; ++i)
	{
		VTXNORTEX vOut;

		if (m_pVIBufferCom.lock()->Get_Vertex(i, &vOut))
			memcpy(&pMeshData->pGroundVertices[i], &vOut, sizeof(VTXNORTEX));
		else
			DEBUG_ASSERT;
	}

	pMeshData->pIndices = shared_ptr<FACEINDICES32[]>(new FACEINDICES32[pMeshData->iNumFaces]);

	for (_uint i = 0; i < pMeshData->iNumFaces; ++i)
	{
		_uint3 Out = { 0, 0, 0 };

		if (m_pVIBufferCom.lock()->Get_Indices(i, &Out))
			memcpy(&pMeshData->pIndices[i], &Out, sizeof(_uint3));
		else
			DEBUG_ASSERT;
	}


	tModelData.Mesh_Datas.push_back(pMeshData);

	tModelData.Bake_Binary();
}

void CEditGround::Load_Mesh()
{
	//Edit Load
	m_pModelData = GAMEINSTANCE->Get_ModelFromKey(m_szMeshName.c_str());

	Remove_Components<CVIBuffer_Ground>();

	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	m_pVIBufferCom.lock()->Init_Mesh(m_pModelData.get()->Mesh_Datas[0]);

	m_bCreate = true;
}

void CEditGround::Load_AllMeshInfo()
{
	fs::directory_iterator itr("../Bin/GroundInfo/");

	string szFileName;

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string().c_str();
		szFileName = szFileName.substr(0, szFileName.size() - 4);

		GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::GROUND, XMMatrixIdentity());

		itr++;
	}
}


void CEditGround::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITINIT == iArg)
	{
		m_bEdit = true;
	}

	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Edit Ground"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			SetUp_Info();
			SetUp_EditMode();
			SetUp_TextureComponent();
			SetUp_ShaderComponent();
		}

		if (ImGui::CollapsingHeader("Edit Picking"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			SetUp_PinckingInfo();
		}

		_char szName[64];
		strcpy_s(szName, m_szMeshName.c_str());

		if (ImGui::InputText("MeshName", szName, 64))
		{
			m_szMeshName = szName;
		}

		if (ImGui::Button("Save"))
		{
			Bake_Mesh();
		}

		if (ImGui::Button("Load"))
		{
			Load_Mesh();
		}
	}
}


void CEditGround::Free()
{
}