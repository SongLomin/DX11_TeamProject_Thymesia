#include "stdafx.h"

#include "EditInstanceProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "Model.h"
#include "Static_Instancing_Prop.h"

#include "Window_PrototypeView.h"
#include "Window_HierarchyView.h"
#include "Window_Optimization_Dev.h"
#include "ImGui_Window.h"
#include "GameManager.h"
#include "imgui.h"

_bool CEditInstanceProp::m_bDetailPicking	= true;
_uint CEditInstanceProp::m_iOption			= 0;

GAMECLASS_C(CEditInstanceProp)
CLONE_C(CEditInstanceProp, CGameObject)

HRESULT CEditInstanceProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditInstanceProp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxModelInstance"),
		VTXMODEL_INSTANCE_DECLARATION::Element,
		VTXMODEL_INSTANCE_DECLARATION::iNumElements
	);

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

	m_pSelect_ShaderCom = Add_Component<CShader>();
	m_pSelect_ShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxCubeTex"),
		VTXCUBETEX_DECLARATION::Element,
		VTXCUBETEX_DECLARATION::iNumElements
	);

	m_pInstanceModelCom   = Add_Component<CVIBuffer_Model_Instance>();
	m_pSelect_VIBufferCom = Add_Component<CVIBuffer_DynamicCube>();

	m_ModelList = GET_SINGLE(CGameInstance)->Get_AllNoneAnimModelKeys();

	ZeroMemory(&m_PickingDesc, sizeof(INSTANCE_MESH_DESC));
	m_PickingDesc.vScale = _float3(1.f, 1.f, 1.f);

	m_pTextureGroupCom.emplace("g_DissolveTexture"    , Add_Component<CTexture>());
	m_pTextureGroupCom.emplace("g_DissolveDiffTexture", Add_Component<CTexture>());

	m_pTextureGroupCom["g_DissolveTexture"].lock()->Use_Texture("T_Fire_Tile_BW_03");
	m_pTextureGroupCom["g_DissolveDiffTexture"].lock()->Use_Texture("Diff_Fire_Tile0");

#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::PRE_LATETICK);
#endif // _USE_THREAD_

	return S_OK;
}

HRESULT CEditInstanceProp::Start()
{
	__super::Start();

	return S_OK;
}

void CEditInstanceProp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bDissolve)
		m_fDissolveRatio += fTimeDelta * m_fDissolveSpeed;
}

void CEditInstanceProp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

void CEditInstanceProp::Thread_PreLateTick(_float fTimeDelta)
{
#ifdef _INSTANCE_CULLING_
	ID3D11DeviceContext* pDeviceContext = GAMEINSTANCE->Get_BeforeRenderContext();

#ifdef _DEBUG
	m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos), GET_SINGLE(CWindow_Optimization_Dev)->Get_InstancingCullingRatio());
#else
	m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos), 0.6f);
#endif // _DEBUG

	m_pInstanceModelCom.lock()->Update_VisibleInstance(pDeviceContext);

	GAMEINSTANCE->Release_BeforeRenderContext(pDeviceContext);
#endif
}

void CEditInstanceProp::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);
}

HRESULT CEditInstanceProp::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(SetUp_ShaderResource(pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

void CEditInstanceProp::Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	string szFileName;

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string();

		if ("" != _szCutName && string::npos != szFileName.find(_szCutName))
		{
			szFileName = szFileName.substr(0, szFileName.find(_szCutName));
		}

		In_List.push_back(szFileName);

		itr++;
	}
}

HRESULT CEditInstanceProp::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };
	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

	if (m_bDissolve)
	{
		for (auto& elem : m_pTextureGroupCom)
		{
			if (FAILED(elem.second.lock()->Set_ShaderResourceView
			(
				m_pShaderCom,
				elem.first.c_str(),
				0
			)))
				return E_FAIL;
		}

		m_pShaderCom.lock()->Set_RawValue("g_fDissolveRatio", &m_fDissolveRatio, sizeof(_float));
	}

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();
	_flag BindTextureFlag;

	static _bool Check[3] = { false, };

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		BindTextureFlag = 0;

		if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			BindTextureFlag |= (1 << aiTextureType_DIFFUSE);

			Check[0] = true;
		}

		if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			BindTextureFlag |= (1 << aiTextureType_NORMALS);

			Check[1] = true;
		}

		if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		{
			BindTextureFlag |= (1 << aiTextureType_SPECULAR);

			Check[2] = true;
		}
		else
		{
			BEGIN_PERFROMANCE_CHECK(m_pInstanceModelCom.lock()->Get_ModelKey());
			END_PERFROMANCE_CHECK(m_pInstanceModelCom.lock()->Get_ModelKey());
		}

		m_iPassIndex = Preset::ShaderPass::ModelInstancingShaderPass(BindTextureFlag, false, m_bNonCulling, false);

		if (0 > m_iPassIndex)
		{
			continue;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
		m_pInstanceModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	SetUp_ShaderResource_Select(pDeviceContext);

	return __super::Render(pDeviceContext);
}

void CEditInstanceProp::SetUp_ShaderResource_Select(ID3D11DeviceContext* pDeviceContext)
{
	if (!m_bSubDraw)
		return;

	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;	

	_matrix PickWorldMatrix = XMMatrixIdentity();
	_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_pPropInfos[m_iPickingIndex].vRotation));

	PickWorldMatrix.r[0] = RotationMatrix.r[0] * m_pPropInfos[m_iPickingIndex].vScale.x;
	PickWorldMatrix.r[1] = RotationMatrix.r[1] * m_pPropInfos[m_iPickingIndex].vScale.y;
	PickWorldMatrix.r[2] = RotationMatrix.r[2] * m_pPropInfos[m_iPickingIndex].vScale.z;
	PickWorldMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&m_pPropInfos[m_iPickingIndex].vTarnslation), 1.f);

	m_pSelect_VIBufferCom.lock()->Update
	(
		m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo,
		PickWorldMatrix
	);

	_float4x4 Matrix;
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());

	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_WorldMatrix", &Matrix, sizeof(_float4x4))))
		return;
	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return;
	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return;

	m_pSelect_ShaderCom.lock()->Begin(1, pDeviceContext);
	m_pSelect_VIBufferCom.lock()->Render(pDeviceContext);
}

void CEditInstanceProp::Update_Instance(INSTANCE_MESH_DESC& In_tDesc)
{
	_vector vPosition;
	_vector vOffsetRange;

	MESH_VTX_INFO tInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

	_vector vMin, vMax, vCenter;
	vMin    = XMLoadFloat3(&tInfo.vMin);
	vMax    = XMLoadFloat3(&tInfo.vMax);
	vCenter = (vMin + vMax) / 2.f;

	vOffsetRange       = XMVectorAbs(XMLoadFloat3(&tInfo.vMax)) + XMVectorAbs(XMLoadFloat3(&tInfo.vMin));
	vOffsetRange      *= XMLoadFloat3(&In_tDesc.vScale);
	In_tDesc.fMaxRange = XMVectorGetX(XMVector3Length(vOffsetRange));
	XMStoreFloat3(&In_tDesc.vCenter, vCenter);
	In_tDesc.Bake_CenterWithMatrix();

	m_pPropInfos.push_back(In_tDesc);
}

void CEditInstanceProp::Update_Instance(_uint In_Index)
{
	_vector vPosition;
	_vector vOffsetRange;

	MESH_VTX_INFO      tInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;
	INSTANCE_MESH_DESC tDesc = m_pPropInfos[In_Index];

	_vector vMin, vMax, vCenter;
	vMin    = XMLoadFloat3(&tInfo.vMin);
	vMax    = XMLoadFloat3(&tInfo.vMax);
	vCenter = (vMin + vMax) / 2.f;

	vOffsetRange       = XMVectorAbs(XMLoadFloat3(&tInfo.vMax)) + XMVectorAbs(XMLoadFloat3(&tInfo.vMin));
	vOffsetRange      *= XMLoadFloat3(&tDesc.vScale);
	tDesc.fMaxRange = XMVectorGetX(XMVector3Length(vOffsetRange));
	XMStoreFloat3(&tDesc.vCenter, vCenter);
	tDesc.Bake_CenterWithMatrix();

	m_pPropInfos[In_Index] = tDesc;
}

void CEditInstanceProp::Write_Json(json& Out_Json)
{
	if ("" == m_szSelectModelName)
	{
		MSG_BOX("Check : m_szSelectModelName value is None");
		return;
	}

	json PropInfo;

	_uint iIndex = 0;
	for (auto& iter : m_pPropInfos)
	{
		_float4x4 PropMatrix;
		ZeroMemory(&PropMatrix, sizeof(_float4x4));

		memcpy(&PropMatrix.m[0], &iter.vRotation.x	 , sizeof(_float3));
		memcpy(&PropMatrix.m[1], &iter.vScale.x		 , sizeof(_float3));
		memcpy(&PropMatrix.m[2], &iter.vTarnslation.x, sizeof(_float3));

		PropInfo.emplace(string("Prop Matrix (" + to_string(iIndex++) + ")"), PropMatrix.m);
	}

	Out_Json.emplace("PropDesc"     , PropInfo);
	Out_Json.emplace("ModelCom"     , m_pInstanceModelCom.lock()->Get_ModelKey());
	Out_Json.emplace("Invisibility" , m_bInvisibility);
	Out_Json.emplace("NonCulling"   , m_bNonCulling);
	Out_Json.emplace("Dissolve"     , m_fDissolveSpeed);
	Out_Json.emplace("Collider_Type", m_iColliderType);
	Out_Json.emplace("SectionIndex" , m_iSectionIndex);

	Out_Json["Hash"] = typeid(CStatic_Instancing_Prop).hash_code();
	Out_Json["Name"] = typeid(CStatic_Instancing_Prop).name();
}

void CEditInstanceProp::Load_FromJson(const json& In_Json)
{
	m_bNonCulling    = In_Json["NonCulling"];
	m_bInvisibility  = In_Json["Invisibility"];
	m_iColliderType  = In_Json["Collider_Type"];
	m_fDissolveSpeed = In_Json["Dissolve"];

	if (In_Json.end() != In_Json.find("SectionIndex"))
	{
		m_iSectionIndex = In_Json["SectionIndex"];
		GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
	}

	string szModelTag = In_Json["ModelCom"];

	if ("" != szModelTag)
	{
		m_pInstanceModelCom.lock()->Init_Model(szModelTag.c_str());
		m_szSelectModelName = szModelTag;
	}

	_vector vPosition;
	_vector vOffsetRange;

	for (auto& iter_item : In_Json["PropDesc"].items())
	{
		_float4x4 PropMatrix;

		CJson_Utility::Load_JsonFloat4x4(iter_item.value(), PropMatrix);

		INSTANCE_MESH_DESC	Desc;
		ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

		memcpy(&Desc.vRotation   , &PropMatrix.m[0][0], sizeof(_float3));
		memcpy(&Desc.vScale      , &PropMatrix.m[1][0], sizeof(_float3));
		memcpy(&Desc.vTarnslation, &PropMatrix.m[2][0], sizeof(_float3));

		vPosition = XMVectorSetW(XMLoadFloat3(&Desc.vTarnslation), 1.f);
				
		Update_Instance(Desc);
	}

	m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
	m_pInstanceModelCom.lock()->Update(m_pPropInfos, true);
}

_bool CEditInstanceProp::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	_float fPickedDist;
	_bool bPicked			= false;
	_uint iIndex			= 0;
	_float4	vCamPosition	= GAMEINSTANCE->Get_CamPosition();
	_vector vCamPos			= XMLoadFloat4(&vCamPosition);

	for (auto& iter : m_pPropInfos)
	{
		if (!m_pInstanceModelCom.lock().get())
		{
			return false;
		}

		MESH_VTX_INFO Info = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		_matrix PickWorldMatrix = iter.Get_Matrix();

		if (SMath::Is_Picked_AbstractCube(In_Ray, Info, PickWorldMatrix, &fPickedDist))
		{
			_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - XMLoadFloat3(&iter.vTarnslation)));

			if (Out_fRange > fLength)
			{
				m_PickingDesc   = iter;
				m_iPickingIndex = iIndex;
				bPicked         = true;
				Out_fRange      = fPickedDist;
			}
		}

		++iIndex;
	}

	return bPicked;
}

void CEditInstanceProp::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
	{
		case EVENT_TYPE::ON_EDITDRAW_ACCEPT:
		{
			m_bSubDraw = true;
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW_NONE:
		{
			m_bSubDraw = false;
		}
		break;

		case EVENT_TYPE::ON_ENTER_SECTION:
		{
			m_bDissolve      = true;
			m_fDissolveRatio = 0.f;
		}
		break;

		case EVENT_TYPE::ON_EXIT_SECTION:
		{
			m_bDissolve      = false;
			m_fDissolveRatio = 0.f;
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW:
		{
			if (!m_bSubDraw)
				return;

			m_pInstanceModelCom.lock()->Update(m_pPropInfos, true);

			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
					View_PhysXOption();
					View_SelectModelComponent();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Pick"))
				{
					View_PickingInfo();

					View_Picking_Prop();
					View_Picking_List();
					View_Picking_Option();
					View_SelectTransformInfo();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Mul Pick"))
				{
					View_MultiPicking();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;

		case EVENT_TYPE::ON_EDIT_PHYSXINFO:
		{
			m_bViewPhysXInfo = true;
		}
		break;

		case EVENT_TYPE::ON_EDIT_PHYSXINFO_N:
		{
			m_bViewPhysXInfo = false;
		}
		break;
	}
}

void CEditInstanceProp::View_SelectModelComponent()
{
	static _int		iSelect_NonAnimModel	 = 0;
	static _char    szFindModelTag[MAX_PATH] = "";
	static _uint    iPropMaxSzie = 0;

	ImGui::Text(string("[ Select MD ] : " + m_szSelectModelName).c_str());
	ImGui::InputText("Find", szFindModelTag, MAX_PATH);

	if (ImGui::BeginListBox("##Model", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (_uint i = 0; i < m_ModelList.size(); ++i)
		{
			const bool is_selected = (iSelect_NonAnimModel == iPropMaxSzie);

			if (0 < strlen(szFindModelTag))
			{
				if (string::npos == m_ModelList[i]->find(szFindModelTag))
					continue;
			}

			if (ImGui::Selectable(m_ModelList[i]->c_str(), is_selected))
			{
				iSelect_NonAnimModel = i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Select", ImVec2(100.f, 25.f)))
	{
		m_szSelectModelName = *m_ModelList[iSelect_NonAnimModel];
		m_pInstanceModelCom.lock()->Init_Model(m_szSelectModelName.c_str());
	}

	ImGui::Checkbox("No CCW", &m_bNonCulling);
	ImGui::Checkbox("Invisibility", &m_bInvisibility);

	ImGui::DragFloat("DissolveSpeed", &m_fDissolveSpeed, 1.f);
	ImGui::InputInt("SectionIndex", &m_iSectionIndex);

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_PickingInfo()
{
	ImGui::Text("");

	if (!m_pPropInfos.empty() && 0 <= m_iPickingIndex && (_int)m_pPropInfos.size() > m_iPickingIndex)
	{
		m_PickingDesc.vTarnslation.y = m_pPropInfos[m_iPickingIndex].vTarnslation.y;
	}

	ImGui::DragFloat3("PickPos" , &m_PickingDesc.vTarnslation.x, 1.f);


	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_Picking_Prop()
{
	ImGui::Checkbox("Detail", &m_bDetailPicking);

	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return;
	
	// 생성
	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if ("" == m_szSelectModelName)
			return;

		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		_float4 vOutPos;
		ZeroMemory(&vOutPos, sizeof(_float4));
		vOutPos.y = m_PickingDesc.vTarnslation.y;

		if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vOutPos))
			return;

		vOutPos.y = m_PickingDesc.vTarnslation.y;
		memcpy(&m_PickingDesc.vTarnslation, &vOutPos, sizeof(_float3));

		Update_Instance(m_PickingDesc);
		m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		m_iPickingIndex = (_uint)m_pPropInfos.size() - 1;
	}

	// 피킹
	else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
	{			
		if (!m_pInstanceModelCom.lock()->Get_ModelData().lock())
			return;

		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		MESH_VTX_INFO VtxInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		_uint   iIndex       = 0;
		_float  fDistance    = 99999999.f;
		_float4	vCamPosition = GAMEINSTANCE->Get_CamPosition();
		_vector vCamPos      = XMLoadFloat4(&vCamPosition);

		for (auto& iter : m_pPropInfos)
		{
			_matrix WorlMatrix     = XMMatrixIdentity();
			_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&iter.vRotation));

			WorlMatrix.r[0] = RotationMatrix.r[0] * iter.vScale.x;
			WorlMatrix.r[1] = RotationMatrix.r[1] * iter.vScale.y;
			WorlMatrix.r[2] = RotationMatrix.r[2] * iter.vScale.z;
			WorlMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&iter.vTarnslation), 1.f);

			if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(WorlMatrix.r[3], iter.vScale.x))
			{
				if (m_bDetailPicking)
				{
					if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, WorlMatrix))
					{
						_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - XMLoadFloat3(&iter.vTarnslation)));

						if (fLength < fDistance)
						{
							fDistance       = fLength;
							m_iPickingIndex = iIndex;
							m_PickingDesc   = m_pPropInfos[iIndex];
						}
					}
				}
				else
				{
					if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, WorlMatrix))
					{
						m_iPickingIndex = iIndex;
						m_PickingDesc   = m_pPropInfos[iIndex];

						break;
					}
				}
			}

			++iIndex;
		}
	}
}

void CEditInstanceProp::View_Picking_List()
{
	ImGui::Text(string(string(" Size : ") + to_string((_uint)m_pPropInfos.size())).c_str());

	if (ImGui::TreeNode("[ Show List ]"))
	{
		if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (_uint i = 0; i < (_uint)m_pPropInfos.size(); ++i)
			{
				const bool is_selected = (m_iPickingIndex == i);

				string szTag = "( " + to_string(i) + " ) " + m_szSelectModelName;

				if (ImGui::Selectable(szTag.c_str(), is_selected))
				{
					m_iPickingIndex = i;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndListBox();
		}

		ImGui::TreePop();
	}

	if (ImGui::Button("Create", ImVec2(100.f, 25.f)) || KEY_INPUT(KEY::M, KEY_STATE::TAP))
	{
		if ("" != m_szSelectModelName)
		{
			Update_Instance(m_PickingDesc);
			m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
			m_iPickingIndex = (_uint)m_pPropInfos.size() - 1;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete", ImVec2(100.f, 25.f)))
	{
		if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
			return;

		auto iter = m_pPropInfos.begin() + m_iPickingIndex;

		if (m_pPropInfos.end() != iter)
		{
			m_pPropInfos.erase(iter);
			m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		}
	}
}

void CEditInstanceProp::View_Picking_Option()
{
	if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
		m_iOption = 0;
	else if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
		m_iOption = 1;
	else if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
		m_iOption = 2;

	static const char* szOptionTag[] =
	{
		"<< Act X >>",
		"<< Act Y >>",
		"<< Act Z >>"
	};

	ImGui::Text("");
	ImGui::Text(string("Select Option : " + string(szOptionTag[m_iOption])).c_str());
	ImGui::Text(string("Pick Index : " + to_string(m_iPickingIndex)).c_str());

	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;

	// Z : 터레인 이동
	// X : 회전(옵션 활성화)
	// C : 수동 이동(옵션 활성화)
	// CTRL + R : 삭제
	// V : 사이즈
	// CTRL + H + X/C/V : 초기화

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);
			_float4 vMouseDir;

			vMouseDir.y = m_PickingDesc.vTarnslation.y;

			if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vMouseDir))
				return;

			_float3 vObjPos = m_pPropInfos[m_iPickingIndex].vTarnslation;
			m_pPropInfos[m_iPickingIndex].vTarnslation = _float3(vMouseDir.x, vObjPos.y, vMouseDir.z);
		}

		else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
			{
				switch (m_iOption)
				{
					case 0: m_pPropInfos[m_iPickingIndex].vRotation.x += 0.01f * MouseMove; break;
					case 1: m_pPropInfos[m_iPickingIndex].vRotation.y += 0.01f * MouseMove; break;
					case 2: m_pPropInfos[m_iPickingIndex].vRotation.z += 0.01f * MouseMove; break;
				}
			}
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
				switch (m_iOption)
				{
					case 0 : m_pPropInfos[m_iPickingIndex].vTarnslation.x -= 0.01f * MouseMove; break;
					case 1 : m_pPropInfos[m_iPickingIndex].vTarnslation.y -= 0.01f * MouseMove; break;
					case 2 : m_pPropInfos[m_iPickingIndex].vTarnslation.z -= 0.01f * MouseMove; break;
				}
			}
		}		
	}

	else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::R, KEY_STATE::TAP))
	{
		auto iter = m_pPropInfos.begin() + m_iPickingIndex;

		if (m_pPropInfos.end() != iter)
		{
			m_pPropInfos.erase(iter);
			m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		}
	}

	else if (KEY_INPUT(KEY::V, KEY_STATE::HOLD))
	{
		switch (m_iOption)
		{
			case 0:
			{
				if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
					m_pPropInfos[m_iPickingIndex].vScale.x -= 0.1f;
				else if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
					m_pPropInfos[m_iPickingIndex].vScale.x += 0.1f;

				Update_Instance(m_iPickingIndex);
			}
			break;

			case 1:
			{
				if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
					m_pPropInfos[m_iPickingIndex].vScale.y -= 0.1f;
				else if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
					m_pPropInfos[m_iPickingIndex].vScale.y += 0.1f;

				Update_Instance(m_iPickingIndex);
			}
			break;

			case 2:
			{
				if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
					m_pPropInfos[m_iPickingIndex].vScale.z -= 0.1f;
				else if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
					m_pPropInfos[m_iPickingIndex].vScale.z += 0.1f;

				Update_Instance(m_iPickingIndex);
			}
			break;
		}
	}

	else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::H, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::X, KEY_STATE::TAP))
		{
			m_pPropInfos[m_iPickingIndex].vRotation = _float3(0.f, 0.f, 0.f);
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::TAP))
		{
			m_pPropInfos[m_iPickingIndex].vTarnslation = _float3(0.f, 0.f, 0.f);
		}

		else if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
		{
			m_pPropInfos[m_iPickingIndex].vScale = _float3(1.f, 1.f, 1.f);
			Update_Instance(m_iPickingIndex);
		}
	}
}

void CEditInstanceProp::View_SelectTransformInfo()
{
	ImGui::Separator();

	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;

	// Position
	ImGui::Text("Position");
	ImGui::DragFloat3("##Position", &m_pPropInfos[m_iPickingIndex].vTarnslation.x, 1.f);
	
	 // Quaternion
	ImGui::Text("Pitch Yaw Roll");
	ImGui::DragFloat3("##Pitch Yaw Roll", &m_pPropInfos[m_iPickingIndex].vRotation.x, 0.01f);

	 // Scale
	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &m_pPropInfos[m_iPickingIndex].vScale.x, 0.1f);

	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_MultiPicking()
{
	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		MESH_VTX_INFO VtxInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		_uint   iIndex       = 0;
		_float  fDistance    = 99999999.f;
		_float4	vCamPosition = GAMEINSTANCE->Get_CamPosition();
		_vector vCamPos      = XMLoadFloat4(&vCamPosition);

		_int   iPickingIndex = -1;
		for (auto& iter : m_pPropInfos)
		{
			auto iter_find = find_if(m_MultPickingIndex.begin(), m_MultPickingIndex.end(), [&](_uint _iIndex)->_bool { return (_iIndex == iIndex); });

			if (iter_find != m_MultPickingIndex.end())
			{
				++iIndex;
				continue;
			}

			_matrix WorlMatrix     = XMMatrixIdentity();
			_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&iter.vRotation));

			WorlMatrix.r[0] = RotationMatrix.r[0] * iter.vScale.x;
			WorlMatrix.r[1] = RotationMatrix.r[1] * iter.vScale.y;
			WorlMatrix.r[2] = RotationMatrix.r[2] * iter.vScale.z;
			WorlMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&iter.vTarnslation), 1.f);

			if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(WorlMatrix.r[3], iter.vScale.x))
			{
				if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, WorlMatrix))
				{
					_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - XMLoadFloat3(&iter.vTarnslation)));

					if (fLength < fDistance)
					{
						fDistance       = fLength;
						iPickingIndex   = iIndex;
					}
				}				
			}

			++iIndex;
		}

		if (0 <= iPickingIndex)
		{
			if (m_MultPickingIndex.empty())
				m_PickingDesc.vTarnslation.y = m_pPropInfos[iPickingIndex].vTarnslation.y;

			m_MultPickingIndex.push_back(iPickingIndex);
		}
	}

	ImGui::Text(string(string("Size : " + to_string(m_MultPickingIndex.size()))).c_str());

	ImGui::InputFloat("Set Pos Y", &m_PickingDesc.vTarnslation.y);

	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
		m_PickingDesc.vTarnslation.y += 0.1f;
	else if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
		m_PickingDesc.vTarnslation.y -= 0.1f;
	if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
		m_PickingDesc.vTarnslation.y += 1.f;
	else if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
		m_PickingDesc.vTarnslation.y -= 1.f;

	if ((ImGui::Button("Clear")) || (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::Y, KEY_STATE::TAP)))
	{
		m_MultPickingIndex.clear();
	}

	else if ((ImGui::Button("Set Y")) || (KEY_INPUT(KEY::Y, KEY_STATE::TAP)))
	{
		for (auto& elem : m_MultPickingIndex)
		{
			m_pPropInfos[elem].vTarnslation.y = m_PickingDesc.vTarnslation.y;
		}
	}

	else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
	{
		_long		MouseMoveZ = 0;
		if (MouseMoveZ = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
		{
			for (auto& elem : m_MultPickingIndex)
			{
				m_pPropInfos[elem].vTarnslation.y -= 0.01f * MouseMoveZ;
			}
		}
	}

}

void CEditInstanceProp::View_PhysXOption()
{
	const _char* items[] = { "None", "Model", "ConvexModel", "Dynamic"};

	if (ImGui::BeginListBox("PhysX Collider Type"))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (m_iColliderType == n);
			if (ImGui::Selectable(items[n], is_selected))
			{
				m_iColliderType = n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CEditInstanceProp::Free()
{
}