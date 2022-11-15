#include "stdafx.h"

#include "EditGroupProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"


#include "Static_Prop.h"
#include "Dynamic_Prop.h"
#include "Interaction_Prop.h"
#include "Light_Prop.h"
#include "Static_Instancing_Prop.h"

#include "Model.h"

#include "Window_PrototypeView.h"
#include "Window_HierarchyView.h"

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

	Load_ResourceList(m_AnimList   , "../Bin/MapTool_MeshInfo/ANIM/"     , ".bin");
	Load_ResourceList(m_NonAnimList, "../Bin/MapTool_MeshInfo/NONANIM/"  , ".bin");
	Load_ResourceList(m_JsonList   , "../Bin/MapTool_MeshInfo/Json_Desc/", ".json");

	m_PropPrototype.emplace(typeid(CStatic_Prop).name()				, PROPS_PROTOTYPE(typeid(CStatic_Prop).hash_code()			 , CStatic_Prop::Create()));
	m_PropPrototype.emplace(typeid(CDynamic_Prop).name()			, PROPS_PROTOTYPE(typeid(CDynamic_Prop).hash_code()			 , CDynamic_Prop::Create()));
	m_PropPrototype.emplace(typeid(CInteraction_Prop).name()		, PROPS_PROTOTYPE(typeid(CInteraction_Prop).hash_code()		 , CInteraction_Prop::Create()));
	m_PropPrototype.emplace(typeid(CLight_Prop).name()				, PROPS_PROTOTYPE(typeid(CLight_Prop).hash_code()			 , CLight_Prop::Create()));
	m_PropPrototype.emplace(typeid(CStatic_Instancing_Prop).name()	, PROPS_PROTOTYPE(typeid(CStatic_Instancing_Prop).hash_code(), CStatic_Instancing_Prop::Create()));

	return S_OK;
}

HRESULT CEditGroupProp::Start()
{
	return S_OK;
}

void CEditGroupProp::Tick(_float fTimeDelta)
{
	for (auto& iter : m_PropList)
	{
		iter.pProp.lock()->Tick(fTimeDelta);
	}
}

void CEditGroupProp::LateTick(_float fTimeDelta)
{
	for (auto& iter : m_PropList)
	{
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

void CEditGroupProp::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW != iArg)
		return;

	// 메쉬, 생성, 삭제
	// 리스트 보기
	// 전체 이동


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

		if (ImGui::BeginTabItem("Edit"))
		{


			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Else"))
		{


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
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
		"CInteraction_Prop",
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
		if (0 == iSelect_ModelType)
			m_szSelectModelName = m_NonAnimList[iSelect_NonAnimModel];

		else if (1 == iSelect_ModelType)
			m_szSelectModelName = m_NonAnimList[iSelect_AnimModel];
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
				for (auto& iter : m_NonAnimList)
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
				for (auto& iter : m_AnimList)
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
	ImGui::Text("[ Mouse Info ] ");
	
	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	ImGui::DragFloat3("##Pos", &MouseRayInWorldSpace.vOrigin.x   , 1.f);
	ImGui::SameLine();
	ImGui::Text("Mouse Pos");

	ImGui::DragFloat3("##Dir", &MouseRayInWorldSpace.vDirection.x, 1.f);
	ImGui::SameLine();
	ImGui::Text("Mouse Dir");
	ImGui::Text("");

	_float3 vPos[4] =
	{
		_float3(   0.f, 0.f, 9999.f),
		_float3(9999.f, 0.f, 9999.f),
		_float3(9999.f, 0.f, 0.f),
		_float3(   0.f, 0.f, 0.f)
	};

	_uint3 iIndex[2] =
	{
		_uint3(0, 1, 2),
		_uint3(0, 2, 3)
	};

	for (_uint i = 0; i < 2; ++i)
	{
		if (0 != isnan(MouseRayInWorldSpace.vOrigin.x))
			break;

		_vector		vPickedPos;
		
		_vector	vVec0 = XMLoadFloat3(&vPos[iIndex[i].ix]);
		_vector	vVec1 = XMLoadFloat3(&vPos[iIndex[i].iy]);
		_vector	vVec2 = XMLoadFloat3(&vPos[iIndex[i].iz]);

		_float fDist  = 0;
		if (DirectX::TriangleTests::Intersects(XMLoadFloat4(&MouseRayInWorldSpace.vOrigin), XMLoadFloat3(&MouseRayInWorldSpace.vDirection), vVec0, vVec1, vVec2, fDist))
		{
			vPickedPos = XMLoadFloat4(&MouseRayInWorldSpace.vOrigin) + XMVector3Normalize(XMLoadFloat3(&MouseRayInWorldSpace.vDirection)) * fDist;
			XMStoreFloat4(&m_vPickingPos, vPickedPos);
		}
	}

	ImGui::Text("[ Picking Info ] ");
	ImGui::DragFloat4("##PickPos", &m_vPickingPos.x, 1.f);
	ImGui::SameLine();
	ImGui::Text("Pick Pos");
	ImGui::Text("");

	string szPickingTag
		= string("[ Obj Info ] : ")
		+ "( " + to_string(m_iPickingIndex) + " )";

	szPickingTag += (0 > m_iPickingIndex || (_int)m_PropList.size() <= m_iPickingIndex)
		? (" None ") 
		: (typeid(m_PropList[m_iPickingIndex]).name());

	ImGui::Text(szPickingTag.c_str());

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{	
		if ("" == m_szSelectModelName)
			return;

		GET_SINGLE(CWindow_PrototypeView)->CallBack_ListClick(typeid(CStatic_Prop).hash_code(), typeid(CStatic_Prop).name());
		weak_ptr<CGameObject> pGameObject = GET_SINGLE(CWindow_HierarchyView)->m_pGameObjects.back().pInstance;
		pGameObject.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
		pGameObject.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));

		/*PROPS_DESC Desc;
		Desc.pProp = GAMEINSTANCE->Add_GameObject<CStatic_Prop>(m_CreatedLevel);
		Desc.pProp.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str(), "");
		Desc.pProp.lock()->Get_Component<CTransform>().lock()->Set_Position(XMLoadFloat4(&m_vPickingPos));*/

		//m_PropList.push_back(Desc);
	}

	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::Pick_Prop()
{
	if (KEY_INPUT(KEY::A, KEY_STATE::TAP))
	{
		if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		{
			RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

			for (_uint i = 0; i < (_uint)m_PropList.size(); ++i)
			{
				m_PropList[i];
			}
		}
	}
}

void CEditGroupProp::Free()
{
}