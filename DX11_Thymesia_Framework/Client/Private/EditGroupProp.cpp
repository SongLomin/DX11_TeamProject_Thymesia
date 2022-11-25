#include "stdafx.h"

#include "EditGroupProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "Shader.h"
#include "Renderer.h"

#include "Static_Prop.h"
#include "Dynamic_Prop.h"
#include "Light_Prop.h"

#include "Model.h"

#include "Window_PrototypeView.h"
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

	Load_ResourceList(m_JsonList   , "../Bin/MapTool_MeshInfo/", ".json");

	return S_OK;
}

HRESULT CEditGroupProp::Start()
{
	return S_OK;
}

void CEditGroupProp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto iter = m_PropList.begin(); iter != m_PropList.end();)
	{
		if (!(*iter).pProp.lock().get())
		{
			iter = m_PropList.erase(iter);
		}
		else
		{
			iter->pProp.lock()->Tick(fTimeDelta);
			iter++;
		}
	}
}

void CEditGroupProp::LateTick(_float fTimeDelta)
{
	for (auto& iter : m_PropList)
	{
		if (iter.pProp.lock())
		{
			iter.pProp.lock()->LateTick(fTimeDelta);

			if (m_bSelect_ShowGroup)
				iter.pProp.lock()->Set_ShaderPass(5);
			else
				iter.pProp.lock()->Set_ShaderPass(0);
		}
	}

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
	if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
		return E_FAIL;

	if (!m_PropList[m_iPickingIndex].pProp.lock())
		return E_FAIL;

	if (!m_bSubDraw)
		return S_OK;

	weak_ptr<CTransform>	pTransform = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();
	weak_ptr<CModel>		pModel     = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CModel>();

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

void CEditGroupProp::Save_Json(string _szName)
{
	json JsonOut;

	_uint iIndex = 0;
	for (auto& iter_prop : m_PropList)
	{
		json PropInfo;

		if (!iter_prop.pProp.lock())
			continue;

		weak_ptr<CModel>	 pModel     = iter_prop.pProp.lock()->Get_Component<CModel>();
		weak_ptr<CTransform> pTransform = iter_prop.pProp.lock()->Get_Component<CTransform>();

		if (!pModel.lock() || !pTransform.lock())
			continue;

		_float4x4 WorldMatrix;
		XMStoreFloat4x4(&WorldMatrix, pTransform.lock()->Get_WorldMatrix());

		PropInfo.emplace("hash"			, iter_prop.hash);
		PropInfo.emplace("ModelKey"		, pModel.lock()->Get_ModelKey());
		PropInfo.emplace("WorldMatrix"  , WorldMatrix.m);

		string szTag = iter_prop.szName + " ( " + to_string(iIndex++) + " )";

		JsonOut.emplace(szTag, PropInfo);
	}

	string szPath = "../Bin/MapTool_MeshInfo/" + _szName +".json";

	if (FAILED(CJson_Utility::Save_Json(szPath.c_str(), JsonOut)))
		return;
}

void CEditGroupProp::Load_Json(string _szName)
{
	json JsonOut;

	string szPath = "../Bin/MapTool_MeshInfo/" + _szName + ".json";

	if (FAILED(CJson_Utility::Load_Json(szPath.c_str(), JsonOut)))
		return;

	for (auto& iter : JsonOut.items())
	{
		json items = iter.value();

		PROPS_DESC	Desc;
		string		ModelKeyTag = "";
		_float4x4	WorldMatrix;
		XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

		for (auto& iter_item : items.items())
		{
			string szKey = iter_item.key();

			if ("hash" == szKey)
			{
				Desc.hash = iter_item.value();

				if (typeid(CStatic_Prop).hash_code() == Desc.hash)
				{
					Desc.pProp  = GAMEINSTANCE->Add_GameObject<CStatic_Prop>(m_CreatedLevel);
					Desc.szName = typeid(CStatic_Prop).name();
				}

				else if (typeid(CDynamic_Prop).hash_code() == Desc.hash)
				{
					Desc.pProp  = GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(m_CreatedLevel);
					Desc.szName = typeid(CDynamic_Prop).name();
				}

				else if ("CLight_Prop" == m_szSelectPropType)
				{
					Desc.pProp  = GAMEINSTANCE->Add_GameObject<CLight_Prop>(m_CreatedLevel);
					Desc.szName = typeid(CLight_Prop).name();
				}

				else
				{
					MSG_BOX("Err : None HashCode");
					return;
				}
			}

			else if ("ModelKey" == szKey)
			{
				ModelKeyTag = iter_item.value();
			}

			else if ("WorldMatrix" == szKey)
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						WorldMatrix.m[i][j] = iter_item.value()[i][j];
					}
				}
			}
		}

		Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(ModelKeyTag.c_str(), "");
		Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_WorldMatrix(XMLoadFloat4x4(&WorldMatrix));

		m_PropList.push_back(Desc);
	}
}

void CEditGroupProp::Write_Json(json& Out_Json)
{
	for (auto& iter_prop : m_PropList)
	{

		CImGui_Window::GAMEOBJECT_DESC Desc;
		Desc.HashCode	= iter_prop.hash;
		Desc.pInstance	= iter_prop.pProp;
		Desc.TypeName	= iter_prop.szName;

		GET_SINGLE(CWindow_HierarchyView)->m_pSubGameObjects.push_back(Desc);
	}
}

_bool CEditGroupProp::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	_float fPickedDist;
	_bool bPicked = false;
	_uint iIndex = 0;

	for (auto& iter : m_PropList)
	{
		weak_ptr<CModel>		pModelCom     = iter.pProp.lock()->Get_Component<CModel>();
		weak_ptr<CTransform>	pTransformCom = iter.pProp.lock()->Get_Component<CTransform>();

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

		case (_uint)EVENT_TYPE::ON_EDITPICKING:
		{
			RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

			for (auto& iter : m_PropList)
			{
				weak_ptr<CModel>		pModelCom     = iter.pProp.lock()->Get_Component<CModel>();
				weak_ptr<CTransform>	pTransformCom = iter.pProp.lock()->Get_Component<CTransform>();

				if (!pModelCom.lock().get() || !pTransformCom.lock().get())
					continue;

				MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

				if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, pTransformCom.lock()->Get_WorldMatrix()))
				{
					CWindow_HierarchyView::GAMEOBJECT_DESC Desc;
					Desc.HashCode	= iter.hash;
					Desc.pInstance	= iter.pProp;
					Desc.TypeName	= iter.szName;

					GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick(Desc);
				}
			}
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW_SUB:
		{
			if (!m_bSubDraw)
			return;

			_uint iIndex = 0;
			static _int iSelect_Info = 0;

			for (auto& iter_prop : m_PropList)
			{
				string szItemTag = (iIndex == m_iPickingIndex) ? ("  =>") : ("  -");

				szItemTag += string(" ( ") + to_string(iIndex) + " ) " + iter_prop.szName;

				if (ImGui::Selectable(szItemTag.c_str()))
				{
					CImGui_Window::GAMEOBJECT_DESC Desc;
					Desc.HashCode	= iter_prop.hash;
					Desc.pInstance	= iter_prop.pProp;
					Desc.TypeName	= iter_prop.szName;

					GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick(Desc);

					m_iPickingIndex = iIndex;
				}

				++iIndex;
			}
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			Pick_Prop();

			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
					View_SelectPropObjectType();
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
					View_SelectJson();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;
	}
}

void CEditGroupProp::View_SelectPropObjectType()
{
	ImGui::Text("");

	static const char* items_PropType[] =
	{
		"CStatic_Prop",
		"CDynamic_Prop",
		"CLight_Prop",
	};

	static _int iSelect_PropType = 0;
	string szFindTag = string("class Client::") + m_szSelectPropType;

	if (ImGui::Combo("Prop Type", &iSelect_PropType, items_PropType, IM_ARRAYSIZE(items_PropType)))
	{
		m_szSelectPropType = items_PropType[iSelect_PropType];
	}

	ImGui::Checkbox("Show Groupe", &m_bSelect_ShowGroup);

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

	SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &m_vPickingPos);

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

	string szPickingTag = string("[ Obj Info ] : ") + "( " + to_string(m_iPickingIndex) + " )";

	szPickingTag += (0 > m_iPickingIndex || (_int)m_PropList.size() <= m_iPickingIndex)
		? (" None ") 
		: (m_PropList[m_iPickingIndex].szName);

	ImGui::Text(szPickingTag.c_str());

	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{	
		if ("" == m_szSelectModelName)
			return;

		if ("CStatic_Prop" == m_szSelectPropType)
		{
			PROPS_DESC Desc;
			Desc.pProp	= GAMEINSTANCE->Add_GameObject<CStatic_Prop>(m_CreatedLevel);
			Desc.hash	= typeid(CStatic_Prop).hash_code();
			Desc.szName	= typeid(CStatic_Prop).name();

			Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
			Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
			m_PropList.push_back(Desc);
		}

		else if ("CDynamic_Prop" == m_szSelectPropType)
		{
			PROPS_DESC Desc;
			Desc.pProp	= GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(m_CreatedLevel);
			Desc.hash	= typeid(CDynamic_Prop).hash_code();
			Desc.szName	= typeid(CDynamic_Prop).name();

			Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
			Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
			m_PropList.push_back(Desc);
		}

		else if ("CLight_Prop" == m_szSelectPropType)
		{
			PROPS_DESC Desc;
			//Desc.pProp	= GAMEINSTANCE->Add_GameObject<CLight_Prop>(m_CreatedLevel);
			Desc.hash	= typeid(CLight_Prop).hash_code();
			Desc.szName	= typeid(CLight_Prop).name();

			Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
			Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
			m_PropList.push_back(Desc);
		}
	}

	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::View_Picking_Prop()
{
	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		_uint iIndex = 0;
		for (auto& iter : m_PropList)
		{
			weak_ptr<CModel>		pModelCom     = iter.pProp.lock()->Get_Component<CModel>();
			weak_ptr<CTransform>	pTransformCom = iter.pProp.lock()->Get_Component<CTransform>();

			if (!pModelCom.lock() || !pTransformCom.lock())
				continue;

			/*if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION)))
				continue;*/

			MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, pTransformCom.lock()->Get_WorldMatrix()))
			{
				m_iPickingIndex = iIndex;
			}

			++iIndex;
		}
	}
}

void CEditGroupProp::View_Picking_List()
{
	if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (_uint i = 0; i < (_uint)m_PropList.size(); ++i)
		{
			const bool is_selected = (m_iPickingIndex == i);

			string szTag = "( " + to_string(i) + " ) " + m_PropList[i].szName;

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

void CEditGroupProp::View_Picking_Option()
{
	if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
		return;

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	_float4 vMouseDir;
	ZeroMemory(&vMouseDir, sizeof(_float4));

	if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vMouseDir))
		return;

	weak_ptr<CTransform>	pTransformCom = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();

	if (!pTransformCom.lock())
		return;

	// Z : 이동, X : 로테이션, 마우스 휠 : y축 이동

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vAddPos = XMVectorSet(vMouseDir.x, vObjPos.m128_f32[1], vMouseDir.z, 1.f);

			pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vAddPos);
		}

		else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vAddPos = XMVectorSet(vMouseDir.x, vObjPos.m128_f32[1], vMouseDir.z, 1.f);

			pTransformCom.lock()->LookAt(vAddPos);
		}
	}
}

void CEditGroupProp::View_Picking_Option_Y()
{
	if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
		return;

	weak_ptr<CTransform>	pTransformCom = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();

	if (!pTransformCom.lock())
		return;

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
				_vector vObjPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
				vObjPos.m128_f32[1] += (MouseMove * -0.01f);

				pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vObjPos);
			}
		}
	}

	_float3 vPos = { 0.f, 0.f, 0.f };
	XMStoreFloat3(&vPos, pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));

	ImGui::DragFloat3("OBJ Pos", &vPos.x, 1.f);
	ImGui::Text("");
}

void CEditGroupProp::View_ModelCopy()
{
	if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
		return;

	if (ImGui::Button("Copy", ImVec2(100.f, 25.f)))
	{
		weak_ptr<CModel> pModel = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CModel>();

		if (!pModel.lock())
			return;

		m_szSelectModelName = pModel.lock()->Get_ModelKey();
	}

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");
}

void CEditGroupProp::View_SelectTransformInfo()
{
	if (m_PropList.empty() || 0 > m_iPickingIndex || m_PropList.size() <= m_iPickingIndex)
		return;

	weak_ptr<CTransform> pTransformCom = m_PropList[m_iPickingIndex].pProp.lock()->Get_Component<CTransform>();

	 // Position
	_vector vPositionVector = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	ImGui::Text("Position");
	ImGui::DragFloat3("##Position", &vPositionVector.m128_f32[0], 1.f);

	pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPositionVector);
	
	 // Quaternion
	_matrix matWorld = pTransformCom.lock()->Get_WorldMatrix();
	_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matWorld));

	ImGui::Text("Pitch Yaw Roll");
	ImGui::DragFloat3("##Pitch Yaw Roll", &vPitchYawRoll.x, 0.01f);
	_vector vQuaternion = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

	pTransformCom.lock()->Rotation_Quaternion(vQuaternion);
	
	 // Scale
	_float3 vScaleFloat3 = pTransformCom.lock()->Get_Scaled();
	_vector vScaleVector = XMLoadFloat3(&vScaleFloat3);

	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &vScaleVector.m128_f32[0], 0.1f);

	vScaleVector.m128_f32[0] = max(0.00001f, vScaleVector.m128_f32[0]);
	vScaleVector.m128_f32[1] = max(0.00001f, vScaleVector.m128_f32[1]);
	vScaleVector.m128_f32[2] = max(0.00001f, vScaleVector.m128_f32[2]);
	XMStoreFloat3(&vScaleFloat3, vScaleVector);

	pTransformCom.lock()->Set_Scaled(vScaleFloat3);
	ImGui::Text("");
}

void CEditGroupProp::View_SelectJson()
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
				{
					++iIndex;
					continue;
				}
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

void CEditGroupProp::Pick_Prop()
{
	if (KEY_INPUT(KEY::ALT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		_uint iIndex = 0;
		for (auto& iter : m_PropList)
		{
			weak_ptr<CModel>		pModelCom     = iter.pProp.lock()->Get_Component<CModel>();
			weak_ptr<CTransform>	pTransformCom = iter.pProp.lock()->Get_Component<CTransform>();

			if (!pModelCom.lock() || !pTransformCom.lock())
				continue;

			/*if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION)))
				continue;*/

			MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, pTransformCom.lock()->Get_WorldMatrix()))
			{
				CImGui_Window::GAMEOBJECT_DESC Desc;
				Desc.HashCode	= iter.hash;
				Desc.pInstance	= iter.pProp;
				Desc.TypeName	= iter.szName;

				GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick(Desc);
				m_iPickingIndex = iIndex;
			}

			++iIndex;
		}
	}
}

void CEditGroupProp::Free()
{
	for (auto& iter : m_PropList)
	{
		iter.pProp.lock()->Set_Dead();
	}
}