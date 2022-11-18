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

	return __super::Render();
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
}

void CEditInstanceProp::OnEventMessage(_uint iArg)
{
	switch (iArg)
	{
		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
					View_SelectModelComponent();
					View_PickingInfo();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Pick"))
				{
					View_Picking_Prop();
					View_Picking_List();
					View_Picking_Option();
					View_Picking_Option_Y();
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
				m_szSelectModelName  = iter;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Select", ImVec2(100.f, 25.f)))
	{
		if ("" != m_szSelectModelName)
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

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD) && KEY_INPUT(KEY::C, KEY_STATE::HOLD))
	{
		_long		MouseMove = 0;
		if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
		{
			m_fPosY += 0.01f * MouseMove;
		}
	}

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
	}
}

void CEditInstanceProp::View_Picking_List()
{
	if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{

		ImGui::EndListBox();
	}
}

void CEditInstanceProp::View_Picking_Option()
{

}

void CEditInstanceProp::View_Picking_Option_Y()
{


	//if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD) && KEY_INPUT(KEY::C, KEY_STATE::HOLD))
	//{
	//	_long		MouseMove = 0;
	//	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	//	{
	//		_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	//		vObjPos.m128_f32[1] += (MouseMove * -0.01f);

	//		pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vObjPos);
	//	}
	//}

	//_float3 vPos = { 0.f, 0.f, 0.f };
	//XMStoreFloat3(&vPos, pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));

	//ImGui::DragFloat3("OBJ Pos", &vPos.x, 1.f);
	//ImGui::Text("");
}

void CEditInstanceProp::View_SelectTransformInfo()
{
	//weak_ptr<CTransform> pTransformCom = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();

	// // Position
	//_vector vPositionVector = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	//ImGui::Text("Position");
	//ImGui::DragFloat3("##Position", &vPositionVector.m128_f32[0], 1.f);

	//pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPositionVector);
	//
	// // Quaternion
	//_matrix matWorld = pTransformCom.lock()->Get_WorldMatrix();
	//_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matWorld));

	//ImGui::Text("Pitch Yaw Roll");
	//ImGui::DragFloat3("##Pitch Yaw Roll", &vPitchYawRoll.x, 0.01f);
	//_vector vQuaternion = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

	//pTransformCom.lock()->Rotation_Quaternion(vQuaternion);
	//
	// // Scale
	//_float3 vScaleFloat3 = pTransformCom.lock()->Get_Scaled();
	//_vector vScaleVector = XMLoadFloat3(&vScaleFloat3);

	//ImGui::Text("Scale");
	//ImGui::DragFloat3("##Scale", &vScaleVector.m128_f32[0], 0.1f);

	//vScaleVector.m128_f32[0] = max(0.00001f, vScaleVector.m128_f32[0]);
	//vScaleVector.m128_f32[1] = max(0.00001f, vScaleVector.m128_f32[1]);
	//vScaleVector.m128_f32[2] = max(0.00001f, vScaleVector.m128_f32[2]);
	//XMStoreFloat3(&vScaleFloat3, vScaleVector);

	//pTransformCom.lock()->Set_Scaled(vScaleFloat3);
	//ImGui::Text("");
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