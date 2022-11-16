#include "stdafx.h"

#include "EditGroupProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"


#include "Static_Prop.h"
#include "Dynamic_Prop.h"
#include "Light_Prop.h"
#include "Static_Instancing_Prop.h"

#include "Model.h"

#include "Window_PrototypeView.h"
#include "Window_HierarchyView.h"
#include "ImGui_Window.h"

GAMECLASS_C(CEditGroupProp)
CLONE_C(CEditGroupProp, CGameObject)

#define D3DCOLOR_ABGR(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

HRESULT CEditGroupProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditGroupProp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_ModelList = GET_SINGLE(CGameInstance)->Get_AllNoneAnimModelKeys();
	//Load_ResourceList(m_JsonList   , "../Bin/MapTool_MeshInfo/Json_Desc/", ".json");

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
			iter.pProp.lock()->LateTick(fTimeDelta);
	}
}

HRESULT CEditGroupProp::Render()
{
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

void CEditGroupProp::Load_MeshList()
{
}

void CEditGroupProp::Load_MashInfo(string _szName)
{
}

void CEditGroupProp::Save_Json(string _szName)
{
}

void CEditGroupProp::Load_Json(string _szName)
{
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

		case (_uint)EVENT_TYPE::ON_EDITDRAW_SUB:
		{
			if (!m_bSubDraw)
			return;

			_uint iIndex = 0;

			for (auto& iter_prop : m_PropList)
			{
				string szItemTag = string("  - ( ") + to_string(iIndex) + " ) " + iter_prop.szName;

				if (ImGui::Selectable(szItemTag.c_str()))
				{
					CImGui_Window::GAMEOBJECT_DESC Desc;
					Desc.HashCode	= iter_prop.hash;
					Desc.pInstance	= iter_prop.pProp;
					Desc.TypeName	= iter_prop.szName;

					GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick(Desc);
				}

				++iIndex;
			}
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			Pick_Prop();

			if (ImGui::BeginTabBar("Map"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
					View_SelectPropObjectType();
					View_EditMode();
					View_SelectModelComponent();
					View_PickingInfo();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Else"))
				{


					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;
	}
}

void CEditGroupProp::View_EditMode()
{
	static const char* items_EditMode[] =
	{
		"MOVE",
		"HEIGHT_LERP",
		"FILLTER",
		"NON"
	};
}

void CEditGroupProp::View_SelectPropObjectType()
{
	ImGui::Text("");

	static const char* items_PropType[] =
	{
		"CStatic_Prop",
		"CDynamic_Prop",
		"CLight_Prop",
		"CStatic_Instancing_Prop"
	};

	static _int iSelect_PropType = 0;
	string szFindTag = string("class Client::") + m_szSelectPropType;

	if (ImGui::Combo("Prop Type", &iSelect_PropType, items_PropType, IM_ARRAYSIZE(items_PropType)))
	{
		m_szSelectPropType = items_PropType[iSelect_PropType];
	}

	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::View_SelectModelComponent()
{
	static const char* ModelType[] =
	{
		"NONANIM",
		"ANIM"
	};

	static _int		iSelect_ModelType		 = 0;
	static _int		iSelect_NonAnimModel	 = 0;
	static _int		iSelect_AnimModel		 = 0;
	static _char    szFindModelTag[MAX_PATH] = "";

	if (ImGui::Combo("Model Type", &iSelect_ModelType, ModelType, IM_ARRAYSIZE(ModelType)))
	{
		if (0 == iSelect_ModelType && !m_ModelList.empty())
			m_szSelectModelName = m_ModelList[iSelect_NonAnimModel];

		else if (1 == iSelect_ModelType && !m_ModelList.empty())
			m_szSelectModelName = m_ModelList[iSelect_AnimModel];
	}
	ImGui::Text("");

	ImGui::Text(string("[ Select MD ] : " + m_szSelectModelName).c_str());
	ImGui::InputText("Find", szFindModelTag, MAX_PATH);
	ImGui::Text("");

	if (ImGui::BeginListBox("Model"))
	{
		_uint iIndex = 0;

		switch (iSelect_ModelType)
		{
			case 0:
			{
				for (auto& iter : m_ModelList)
				{
					const bool is_selected = (iSelect_NonAnimModel == iIndex);

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
			}
			break;

			case 1:
			{
				for (auto& iter : m_ModelList)
				{
					const bool is_selected = (iSelect_AnimModel == iIndex);

					if (0 < strlen(szFindModelTag))
					{
						if (string::npos == iter.find(szFindModelTag))
							continue;
					}

					if (ImGui::Selectable(iter.c_str(), is_selected))
					{
						iSelect_AnimModel   = iIndex;
						m_szSelectModelName = iter;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();

					++iIndex;
				}
			}
			break;
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

	SMath::Is_Picked(MouseRayInWorldSpace, &m_vPickingPos);

	_long iMouseWheel = 0;
	if (iMouseWheel = GAMEINSTANCE->Get_DIMouseMoveState(MOUSEMOVE::MMS_WHEEL))
	{
		m_fPosY += 0.01f * (abs(iMouseWheel) / iMouseWheel);
	}

	m_vPickingPos.y = m_fPosY;


	ImGui::Text("[ Picking Info ] ");
	ImGui::DragFloat4("##PickPos", &m_vPickingPos.x, 1.f);
	ImGui::SameLine();
	ImGui::Text("Pick Pos");

	string szPickingTag = string("[ Obj Info ] : ") + "( " + to_string(m_iPickingIndex) + " )";

	szPickingTag += (0 > m_iPickingIndex || (_int)m_PropList.size() <= m_iPickingIndex)
		? (" None ") 
		: (typeid(m_PropList[m_iPickingIndex]).name());

	ImGui::Text(szPickingTag.c_str());

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
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

		else if ("CStatic_Instancing_Prop" == m_szSelectPropType)
		{
			PROPS_DESC Desc;
			//Desc.pProp	= GAMEINSTANCE->Add_GameObject<CStatic_Instancing_Prop>(m_CreatedLevel);
			Desc.hash	= typeid(CStatic_Instancing_Prop).hash_code();
			Desc.szName	= typeid(CStatic_Instancing_Prop).name();

			Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
			Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));
			m_PropList.push_back(Desc);
		}
	}

	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::Pick_Prop()
{
	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		for (auto& iter : m_PropList)
		{
			weak_ptr<CModel>		pModelCom     = iter.pProp.lock()->Get_Component<CModel>();
			weak_ptr<CTransform>	pTransformCom = iter.pProp.lock()->Get_Component<CTransform>();

			if (!pModelCom.lock() || !pTransformCom.lock())
			{
				continue;
			}

			MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

			if (Check_Click(MouseRayInWorldSpace, Info, pTransformCom.lock()->Get_WorldMatrix()))
			{
				CImGui_Window::GAMEOBJECT_DESC Desc;
				Desc.HashCode	= iter.hash;
				Desc.pInstance	= iter.pProp;
				Desc.TypeName	= iter.szName;

				GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick(Desc);
			}
		}
	}
}

_bool CEditGroupProp::Check_Click(RAY _Ray, MESH_VTX_INFO _VtxInfo, _matrix _WorldMatrix)
{
	_matrix		WorldMatrixInv = XMMatrixInverse(nullptr, _WorldMatrix);

	_vector vRayPos = XMVector3TransformCoord(XMLoadFloat4(&_Ray.vOrigin), WorldMatrixInv);
	_vector vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&_Ray.vDirection), WorldMatrixInv));

	_float3				vPosition[8];
	_uint3				vIndex[12];

	vPosition[0] = { _VtxInfo.vMin.x, _VtxInfo.vMax.y, _VtxInfo.vMin.z };
	vPosition[1] = { _VtxInfo.vMax.x, _VtxInfo.vMax.y, _VtxInfo.vMin.z };
	vPosition[2] = { _VtxInfo.vMax.x, _VtxInfo.vMin.y, _VtxInfo.vMin.z };
	vPosition[3] = { _VtxInfo.vMin.x, _VtxInfo.vMin.y, _VtxInfo.vMin.z };
	vPosition[4] = { _VtxInfo.vMin.x, _VtxInfo.vMax.y, _VtxInfo.vMax.z };
	vPosition[5] = { _VtxInfo.vMax.x, _VtxInfo.vMax.y, _VtxInfo.vMax.z };
	vPosition[6] = { _VtxInfo.vMax.x, _VtxInfo.vMin.y, _VtxInfo.vMax.z };
	vPosition[7] = { _VtxInfo.vMin.x, _VtxInfo.vMin.y, _VtxInfo.vMax.z };

	vIndex[0]  = { 1, 5, 6 };
	vIndex[1]  = { 1, 6, 2 };
	vIndex[2]  = { 4, 0, 3 };
	vIndex[3]  = { 4, 3, 7 };
	vIndex[4]  = { 4, 5, 1 };
	vIndex[5]  = { 4, 1, 0 };
	vIndex[6]  = { 3, 2, 6 };
	vIndex[7]  = { 3, 6, 7 };
	vIndex[8]  = { 5, 4, 7 };
	vIndex[9]  = { 5, 7, 6 };
	vIndex[10] = { 0, 1, 2 };
	vIndex[11] = { 0, 2, 3 };

	_float		fDist;

	for (_uint i = 0; i < 12; ++i)
	{
		_vector	vVec0 = XMLoadFloat3(&vPosition[vIndex[i].ix]);
		_vector	vVec1 = XMLoadFloat3(&vPosition[vIndex[i].iy]);
		_vector	vVec2 = XMLoadFloat3(&vPosition[vIndex[i].iz]);

		if (true == DirectX::TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
		{
			return true;
		}
	}

	return false;
}

void CEditGroupProp::Free()
{
}