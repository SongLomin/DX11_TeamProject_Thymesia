#include "stdafx.h"

#include "EditGroupProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"

#include "Client_GameObjects.h"

#include "Window_HierarchyView.h"
#include "ImGui_Window.h"

GAMECLASS_C(CEditGroupProp)
CLONE_C(CEditGroupProp, CGameObject)

HRESULT CEditGroupProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditGroupProp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxCubeTex"),
		VTXCUBETEX_DECLARATION::Element,
		VTXCUBETEX_DECLARATION::iNumElements
	);

	m_pVIBufferCom = Add_Component<CVIBuffer_DynamicCube>();
	m_ModelList    = GET_SINGLE(CGameInstance)->Get_AllNoneAnimModelKeys();

	return S_OK;
}

HRESULT CEditGroupProp::Start()
{
	return S_OK;
}

void CEditGroupProp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEditGroupProp::LateTick(_float fTimeDelta)
{
	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Cast<CGameObject>(m_this));
}

HRESULT CEditGroupProp::Render()
{
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

void CEditGroupProp::Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName)
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

HRESULT CEditGroupProp::SetUp_ShaderResource()
{
	if (!m_bSubDraw)
		return S_OK;

	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return E_FAIL;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return E_FAIL;

	weak_ptr<CTransform>	pTransform = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();
	weak_ptr<CModel>		pModel     = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CModel>();

	if (!pModel.lock() || !pTransform.lock())
		return E_FAIL;

	m_pVIBufferCom.lock()->Update
	(
		pModel.lock()->Get_MeshVertexInfo(),
		pTransform.lock()->Get_WorldMatrix()
	);

	_float4x4 Matrix;
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &Matrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom.lock()->Begin(1);
	m_pVIBufferCom.lock()->Render();

	return S_OK;
}

void CEditGroupProp::Write_Json(json& Out_Json)
{
}

_bool CEditGroupProp::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return false;

	_float fPickedDist;
	_bool bPicked = false;
	_uint iIndex  = 0;

	for (auto& iter : iter_collider->second)
	{
		weak_ptr<CModel>		pModelCom     = iter.pInstance.lock()->Get_Component<CModel>();
		weak_ptr<CTransform>	pTransformCom = iter.pInstance.lock()->Get_Component<CTransform>();

		if (!pModelCom.lock().get() || !pTransformCom.lock().get())
		{
			++iIndex;
			continue;
		}

		MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		if (SMath::Is_Picked_AbstractCube(In_Ray, Info, pTransformCom.lock()->Get_WorldMatrix(), &fPickedDist))
		{
			if (Out_fRange > fPickedDist)
			{
				Out_fRange      = fPickedDist;
				m_iPickingIndex = iIndex;
				bPicked = true;
			}
		}

		++iIndex;
	}

	return bPicked;
}

void CEditGroupProp::OnEventMessage(_uint iArg)
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
			if (!m_bSubDraw)
				return;

			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
					View_CreateProp();
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
					View_ModelCopy();
					View_SelectTransformInfo();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Save & Load"))
				{

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;
	}
}

void CEditGroupProp::View_CreateProp()
{
	ImGui::Text("");

	static const char* items_PropType[] =
	{
		"Interaction_Prop",
		"Dynamic_Prop",
		"Light_Prop",
	};

	static const char* items_Interaction_Prop[] =
	{
		"Door",
		"CheckPoint",
		"Elevator",
		"Ladder"
	};

	static _int iSelect_PropType = 0;
	static _int iSelect_PropName = 0;

	if (ImGui::Combo("Prop Type", &iSelect_PropType, items_PropType, IM_ARRAYSIZE(items_PropType)))
	{
		iSelect_PropName = 0;
	}

	if (0 == iSelect_PropType)
		ImGui::Combo("Prop Name", &iSelect_PropName, items_Interaction_Prop, IM_ARRAYSIZE(items_Interaction_Prop));

	RAY MouseRayInWorldSpace;

	if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
		return;

	if (0 == iSelect_PropType)
	{
		switch (iSelect_PropName)
		{
			case 0:
			{
				weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CInteraction_Door>(LEVEL::LEVEL_EDIT);
			}
			break;

			case 1:
			{
				weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CInteraction_CheckPoint>(LEVEL::LEVEL_EDIT);
			}
			break;

			case 2:
			{
				weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CInteraction_Elevator>(LEVEL::LEVEL_EDIT);
			}
			break;

			case 3:
			{
				weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CInteraction_Ladder>(LEVEL::LEVEL_EDIT);
			}
			break;
		}
	}
	else
	{
	}

	

	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::View_SelectModelComponent()
{
	static _int		iSelect_NonAnimModel	 = 0;
	static _char    szFindModelTag[MAX_PATH] = "";

	ImGui::Text(string("[ Select MD ] : " + m_szSelectModelName).c_str());
	ImGui::InputText("Find", szFindModelTag, MAX_PATH);
	ImGui::Text("");

	if (ImGui::BeginListBox("##Model", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		_uint iIndex = 0;

		for (auto& iter : m_ModelList)
		{
			const bool is_selected = (iSelect_NonAnimModel == iIndex);

			if (0 < strlen(szFindModelTag))
			{
				if (string::npos == iter.find(szFindModelTag))
				{
					++iIndex;
					continue;
				}
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

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::View_PickingInfo()
{
	//RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	//if (ImGui::TreeNode("[ Mouse Info ] "))
	//{
	//	ImGui::DragFloat3("##Pos", &MouseRayInWorldSpace.vOrigin.x, 1.f);
	//	ImGui::SameLine();
	//	ImGui::Text("Mouse Pos");

	//	ImGui::DragFloat3("##Dir", &MouseRayInWorldSpace.vDirection.x, 1.f);
	//	ImGui::SameLine();
	//	ImGui::Text("Mouse Dir");

	//	ImGui::TreePop();
	//}

	//ImGui::Text("");

	//SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &m_vPickingPos);

	//if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD) && KEY_INPUT(KEY::C, KEY_STATE::HOLD))
	//{
	//	_long		MouseMove = 0;
	//	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	//	{
	//		m_fPosY += 0.01f * MouseMove;
	//	}
	//}

	//m_vPickingPos.y = m_fPosY;

	//ImGui::Text("[ Picking Info ] ");
	//ImGui::DragFloat4("##PickPos", &m_vPickingPos.x, 1.f);
	//ImGui::SameLine();
	//ImGui::Text("Pick Pos");

	//string szPickingTag = string("[ Obj Info ] : ") + "( " + to_string(m_iPickingIndex) + " )";

	//szPickingTag += (0 > m_iPickingIndex || (_int)m_PropList.size() <= m_iPickingIndex)
	//	? (" None ") 
	//	: (m_PropList[m_iPickingIndex].szName);

	//ImGui::Text(szPickingTag.c_str());

	//if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	//{	
	//	if ("" == m_szSelectModelName)
	//		return;

	//	if ("CStatic_Prop" == m_szSelectPropType)
	//	{
	//		PROPS_DESC Desc;
	//		Desc.pProp	= GAMEINSTANCE->Add_GameObject<CStatic_Prop>(m_CreatedLevel);
	//		Desc.hash	= typeid(CStatic_Prop).hash_code();
	//		Desc.szName	= typeid(CStatic_Prop).name();

	//		Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
	//		Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
	//		m_PropList.push_back(Desc);
	//	}

	//	else if ("CDynamic_Prop" == m_szSelectPropType)
	//	{
	//		PROPS_DESC Desc;
	//		Desc.pProp	= GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(m_CreatedLevel);
	//		Desc.hash	= typeid(CDynamic_Prop).hash_code();
	//		Desc.szName	= typeid(CDynamic_Prop).name();

	//		Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
	//		Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
	//		m_PropList.push_back(Desc);
	//	}

	//	else if ("CLight_Prop" == m_szSelectPropType)
	//	{
	//		PROPS_DESC Desc;
	//		//Desc.pProp	= GAMEINSTANCE->Add_GameObject<CLight_Prop>(m_CreatedLevel);
	//		Desc.hash	= typeid(CLight_Prop).hash_code();
	//		Desc.szName	= typeid(CLight_Prop).name();

	//		Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
	//		Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
	//		m_PropList.push_back(Desc);
	//	}
	//}

	//ImGui::Text("");
	//ImGui::Separator();
}

void CEditGroupProp::View_Picking_Prop()
{
	//if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	//{
	//	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	//	_uint iIndex = 0;
	//	for (auto& iter : m_PropList)
	//	{
	//		weak_ptr<CModel>		pModelCom     = iter.pProp.lock()->Get_Component<CModel>();
	//		weak_ptr<CTransform>	pTransformCom = iter.pProp.lock()->Get_Component<CTransform>();

	//		if (!pModelCom.lock() || !pTransformCom.lock())
	//			continue;

	//		/*if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION)))
	//			continue;*/

	//		MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

	//		if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, pTransformCom.lock()->Get_WorldMatrix()))
	//		{
	//			m_iPickingIndex = iIndex;
	//		}

	//		++iIndex;
	//	}
	//}
}

void CEditGroupProp::View_Picking_List()
{
	//if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	//{
	//	for (_uint i = 0; i < (_uint)m_PropList.size(); ++i)
	//	{
	//		const bool is_selected = (m_iPickingIndex == i);

	//		string szTag = "( " + to_string(i) + " ) " + m_PropList[i].szName;

	//		if (ImGui::Selectable(szTag.c_str(), is_selected))
	//		{
	//			m_iPickingIndex = i;
	//		}

	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}

	//	ImGui::EndListBox();
	//}
}

void CEditGroupProp::View_Picking_Option()
{
	//if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
	//	return;

	//RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	//_float4 vMouseDir;
	//ZeroMemory(&vMouseDir, sizeof(_float4));

	//if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vMouseDir))
	//	return;

	//weak_ptr<CTransform>	pTransformCom = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();

	//if (!pTransformCom.lock())
	//	return;

	//// Z : 이동, X : 로테이션, 마우스 휠 : y축 이동

	//if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	//{
	//	if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
	//	{
	//		_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	//		_vector vAddPos = XMVectorSet(vMouseDir.x, vObjPos.m128_f32[1], vMouseDir.z, 1.f);

	//		pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vAddPos);
	//	}

	//	else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
	//	{
	//		_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	//		_vector vAddPos = XMVectorSet(vMouseDir.x, vObjPos.m128_f32[1], vMouseDir.z, 1.f);

	//		pTransformCom.lock()->LookAt(vAddPos);
	//	}
	//}
}

void CEditGroupProp::View_Picking_Option_Y()
{
	//if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
	//	return;

	//weak_ptr<CTransform>	pTransformCom = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();

	//if (!pTransformCom.lock())
	//	return;

	//if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	//{
	//	if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
	//	{
	//		_long		MouseMove = 0;
	//		if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	//		{
	//			_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	//			vObjPos.m128_f32[1] += (MouseMove * -0.01f);

	//			pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vObjPos);
	//		}
	//	}
	//}

	//_float3 vPos = { 0.f, 0.f, 0.f };
	//XMStoreFloat3(&vPos, pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));

	//ImGui::DragFloat3("OBJ Pos", &vPos.x, 1.f);
	//ImGui::Text("");
}

void CEditGroupProp::View_ModelCopy()
{
	//if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
	//	return;

	//if (ImGui::Button("Copy", ImVec2(100.f, 25.f)))
	//{
	//	weak_ptr<CModel> pModel = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CModel>();

	//	if (!pModel.lock())
	//		return;

	//	m_szSelectModelName = pModel.lock()->Get_ModelKey();
	//}

	//ImGui::Text("");
	//ImGui::Separator();
	//ImGui::Text("");
}

void CEditGroupProp::View_SelectTransformInfo()
{
	//if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
	//	return;

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

_bool CEditGroupProp::Pick_Prop(RAY& _pMouseRayInWorldSpace)
{
	return false;
}

void CEditGroupProp::Free()
{

}