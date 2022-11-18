#include "stdafx.h"

#include "EditInstanceProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"

#include "Model.h"
#include "Static_Instancing_Prop.h"

#include "Window_PrototypeView.h"
#include "Window_HierarchyView.h"
#include "ImGui_Window.h"

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

	m_pInstanceModelCom.lock()->Update(m_pPropInfos);
}

void CEditInstanceProp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CEditInstanceProp::Render()
{
	if (FAILED(SetUp_ShaderResource()))
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

HRESULT CEditInstanceProp::SetUp_ShaderResource()
{
	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_vector vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			DEBUG_ASSERT;
		}

		if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			m_iPassIndex = 0;
		}
		else
		{
			m_iPassIndex = 1;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pInstanceModelCom.lock()->Render_Mesh(i);
	}

	SetUp_ShaderResource_Select();


	return __super::Render();
}

void CEditInstanceProp::SetUp_ShaderResource_Select()
{
	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;
	
	if (!m_bSubDraw)
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

	m_pSelect_ShaderCom.lock()->Begin(1);
	m_pSelect_VIBufferCom.lock()->Render();
}

void CEditInstanceProp::Save_Json(string _szName)
{
	json JsonOut;
}

void CEditInstanceProp::Load_Json(string _szName)
{
	json JsonOut;
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

		memcpy(&PropMatrix.m[0], &iter.vRotation.x		, sizeof(_float3));
		memcpy(&PropMatrix.m[1], &iter.vScale.x			, sizeof(_float3));
		memcpy(&PropMatrix.m[2], &iter.vTarnslation.x	, sizeof(_float3));

		PropInfo.emplace(string("Prop Matrix (" + to_string(iIndex++) + ")"), PropMatrix.m);
	}

	Out_Json.emplace("PropDesc", PropInfo);
	Out_Json.emplace("ModelCom", m_pInstanceModelCom.lock()->Get_ModelKey());

	if (Out_Json.end() != Out_Json.find("Hash"))
	{
		Out_Json["Hash"] = typeid(CStatic_Instancing_Prop).hash_code();
	}

	if (Out_Json.end() != Out_Json.find("Name"))
	{
		Out_Json["Name"] = typeid(CStatic_Instancing_Prop).name();
	}
}

void CEditInstanceProp::Load_FromJson(const json& In_Json)
{
	for (auto& iter : In_Json.items())
	{
		string szKey = iter.key();

		if ("ModelCom" == szKey)
		{
			string szModelTag = iter.value();
		}
		else if ("PropDesc" == szKey)
		{
			json Desc = iter.value();

			for (auto& iter_item : Desc.items())
			{
				_float4x4 PropMatrix;

				CJson_Utility::Load_JsonFloat4x4(iter_item.value(), PropMatrix);

				INSTANCE_MESH_DESC	Desc;
				ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

				memcpy(&Desc.vRotation   , &PropMatrix.m[0][0], sizeof(_float3));
				memcpy(&Desc.vScale      , &PropMatrix.m[1][0], sizeof(_float3));
				memcpy(&Desc.vTarnslation, &PropMatrix.m[2][0], sizeof(_float3));

				m_pPropInfos.push_back(Desc);
			}
		}
	}
}

void CEditInstanceProp::OnEventMessage(_uint iArg)
{
	switch (iArg)
	{
		case (_uint)EVENT_TYPE::ON_EDITDRAW_ACCEPT:
		{
			m_bSubDraw = true;
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW_NONE:
		{
			m_bSubDraw = false;
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
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

				if (ImGui::BeginTabItem("Save & Load"))
				{
					View_SelectJson();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
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
		_uint iIndex = 0;

		for (auto& iter : m_ModelList)
		{
			const bool is_selected = (iSelect_NonAnimModel == iPropMaxSzie);

			if (0 < strlen(szFindModelTag))
			{
				if (string::npos == iter.find(szFindModelTag))
					continue;
			}

			if (ImGui::Selectable(iter.c_str(), is_selected))
			{
				iSelect_NonAnimModel = iIndex;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Select", ImVec2(100.f, 25.f)))
	{
		m_szSelectModelName = m_ModelList[iSelect_NonAnimModel];
		m_pInstanceModelCom.lock()->Init_Model(m_szSelectModelName.c_str());
	}

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_PickingInfo()
{
	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	if (ImGui::TreeNode("[ Mouse Info ] "))
	{
		ImGui::DragFloat3("##Pos", &MouseRayInWorldSpace.vOrigin.x, 1.f);
		ImGui::SameLine();
		ImGui::Text("Mouse Pos");

		ImGui::DragFloat3("##Dir", &MouseRayInWorldSpace.vDirection.x, 1.f);
		ImGui::SameLine();
		ImGui::Text("Mouse Dir");

		ImGui::TreePop();
	}

	ImGui::Text("");

	SMath::Is_Picked(MouseRayInWorldSpace, &m_vPickingPos);

	ImGui::Text("");

	m_vPickingPos.y = m_fPosY;

	ImGui::Text("[ Picking Info ] ");
	ImGui::DragFloat4("##PickPos", &m_vPickingPos.x, 1.f);
	ImGui::SameLine();
	ImGui::Text("Pick Pos");

	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{	
		if ("" == m_szSelectModelName)
			return;

		INSTANCE_MESH_DESC	Desc;
		Desc.vRotation		= _float3(0.f, 0.f, 0.f);
		Desc.vScale			= _float3(1.f, 1.f, 1.f);
		Desc.vTarnslation	= _float3(m_vPickingPos.x, m_vPickingPos.y, m_vPickingPos.z);

		m_pPropInfos.push_back(Desc);
		m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
	}

	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_Picking_Prop()
{
	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		if (!m_pInstanceModelCom.lock()->Get_ModelData().lock())
			return;

		MESH_VTX_INFO VtxInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;
		
		_uint iIndex = 0;
		for (auto& iter : m_pPropInfos)
		{
			_matrix WorlMatrix     = XMMatrixIdentity();
			_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&iter.vRotation));

			WorlMatrix.r[0] = RotationMatrix.r[0] * iter.vScale.x;
			WorlMatrix.r[1] = RotationMatrix.r[1] * iter.vScale.y;
			WorlMatrix.r[2] = RotationMatrix.r[2] * iter.vScale.z;
			WorlMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&iter.vTarnslation), 1.f);

			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, WorlMatrix))
			{
				m_iPickingIndex = iIndex;
				m_fPosY = m_pPropInfos[iIndex].vTarnslation.y;
			}

			++iIndex;
		}
	}
}

void CEditInstanceProp::View_Picking_List()
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
}

void CEditInstanceProp::View_Picking_Option()
{
	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	_float4 vMouseDir;
	ZeroMemory(&vMouseDir, sizeof(_float4));

	if (!SMath::Is_Picked(MouseRayInWorldSpace, &vMouseDir))
		return;

	// Z : 이동, X : 로테이션, 마우스 휠 : y축 이동
	// C : y 값 변경


	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
			{
				m_pPropInfos[m_iPickingIndex].vRotation.y += 0.01f * MouseMove;
			}
		}

		else if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			_float3 vObjPos = m_pPropInfos[m_iPickingIndex].vTarnslation;
			m_pPropInfos[m_iPickingIndex].vTarnslation = _float3(vMouseDir.x, vObjPos.y, vMouseDir.z);
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
				m_pPropInfos[m_iPickingIndex].vTarnslation.y -= 0.01f * MouseMove;
				m_fPosY = m_pPropInfos[m_iPickingIndex].vTarnslation.y;
			}
		}
	}
}

void CEditInstanceProp::View_SelectTransformInfo()
{
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
}

void CEditInstanceProp::View_SelectJson()
{
	static _char szJsonFileTag[MAX_PATH] = "";
	ImGui::InputText("Name", szJsonFileTag, MAX_PATH);

	static int	iSelect_MeshData = 0;
	if (ImGui::BeginListBox("##Json List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		_uint iIndex = 0;

		for (auto& iter : m_JsonList)
		{
			const bool is_selected = (iSelect_MeshData == iIndex);

			if (0 < strlen(szJsonFileTag))
			{
				if (string::npos == m_JsonList[iIndex].find(szJsonFileTag))
					continue;
			}

			if (ImGui::Selectable(iter.c_str(), is_selected))
			{
				iSelect_MeshData = iIndex;
				strcpy_s(szJsonFileTag, m_JsonList[iIndex].c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Save", ImVec2(100.f, 25.f)) && !m_JsonList.empty())
	{
		Save_Json(m_JsonList[iSelect_MeshData]);
	}

	ImGui::SameLine();

	if (ImGui::Button("Load", ImVec2(100.f, 25.f)) && !m_JsonList.empty())
	{
		Load_Json(m_JsonList[iSelect_MeshData]);
	}
}

void CEditInstanceProp::Free()
{
}