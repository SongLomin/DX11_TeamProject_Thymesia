#include "stdafx.h"
#include "Client_Windows.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"

#include "Model.h"
#include "VIBuffer_Model_Instance.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CWindow_HierarchyView)

HRESULT CWindow_HierarchyView::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC("Hierarchy_View", ImVec2(300.f, 500.f), window_flags);
	
	return S_OK;
}

void CWindow_HierarchyView::Start()
{
	GET_SINGLE(CWindow_PrototypeView)->CallBack_ListClick += bind(&CWindow_HierarchyView::Call_Add_GameObject, this, placeholders::_1, placeholders::_2);
}

void CWindow_HierarchyView::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto iter = m_pGameObjects.begin(); iter != m_pGameObjects.end();)
	{
		if (!(*iter).pInstance.lock().get())
		{
			iter = m_pGameObjects.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	Picking_Obj();

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) && KEY_INPUT(KEY::S, KEY_STATE::TAP))
	{
		json NewJson;
		Write_Json(NewJson);

		if (!NewJson.empty())
		{
			time_t timer = time(NULL);
			tm     TimeDesc;

			localtime_s(&TimeDesc, &timer);

			string szPath
				= string("../Bin/LevelData/AutoSave/AutoSave ")
				+ to_string(TimeDesc.tm_mon) + "."
				+ to_string(TimeDesc.tm_wday) + " ("
				+ to_string(TimeDesc.tm_hour) + "-"
				+ to_string(TimeDesc.tm_min) + "-"
				+ to_string(TimeDesc.tm_sec) + ").json";

			CJson_Utility::Save_Json(szPath.c_str(), NewJson);
		}
	}
}

HRESULT CWindow_HierarchyView::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	
	static char _szFindTag[MAX_PATH] = "";
	ImGui::InputText("##Find", _szFindTag, MAX_PATH);

	_uint iIndex = 0;

	for (auto& elem : m_pGameObjects)
	{
		string szIndexedName = to_string(iIndex) + ". " + elem.TypeName.substr(string("class Client::").length());

		if (m_iPreSelectIndex == iIndex)
			szIndexedName = "[ " + szIndexedName + " ]";

		weak_ptr<CVIBuffer_Model_Instance> pModel = elem.pInstance.lock()->Get_Component<CVIBuffer_Model_Instance>().lock();

		if (pModel.lock() && "" != pModel.lock()->Get_ModelKey())
		{
			szIndexedName += string("\n      >> ") + pModel.lock()->Get_ModelKey() + "\n";

			if (0 != strlen(_szFindTag) && string::npos == pModel.lock()->Get_ModelKey().find(_szFindTag))
			{
				++iIndex;
				continue;
			}
		}

		if (ImGui::Selectable(szIndexedName.c_str()))
		{		
			CallBack_ListClick(elem);

			m_pGameObjects[m_iPreSelectIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_NONE);
			elem.pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_ACCEPT);

			m_iPreSelectIndex = iIndex;
		}

		if ((m_iPreSelectIndex == iIndex) && (elem.HashCode == typeid(CEditGroupProp).hash_code()))
		{
			elem.pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_SUB);
		}

		++iIndex;
	}
	
	__super::End();

	return S_OK;
}

void CWindow_HierarchyView::Call_Add_GameObject(const _hashcode& TypeHash, const char* TypeName)
{
	Call_Add_GameObject_Internal(TypeHash, TypeName);
}

void CWindow_HierarchyView::Write_Json(json& Out_Json)
{
	_uint iIndex = 0;
	vector<string> ModelTag;

	auto iter_elem = m_pGameObjects.begin();

	while (iter_elem != m_pGameObjects.end())
	{
		if (typeid(CEditGroupProp).hash_code() == iter_elem->HashCode)
		{
			json EmptyJson;

			iter_elem->pInstance.lock()->Write_Json(EmptyJson);

			for (auto iter_sub : m_pSubGameObjects)
			{
				Out_Json["GameObject"][iIndex]["Name"]				= iter_sub.TypeName;
				Out_Json["GameObject"][iIndex]["Hash"]				= iter_sub.HashCode;
				Out_Json["GameObject"][iIndex]["Setting"]["Enable"] = iter_sub.pInstance.lock()->Get_Enable();
				Out_Json["GameObject"][iIndex]["Component"]["Transform"].emplace();

				iter_sub.pInstance.lock()->Write_Json(Out_Json["GameObject"][iIndex]);

				weak_ptr<CModel> pModel = iter_sub.pInstance.lock()->Get_Component<CModel>();

				if (pModel.lock().get())
				{
					string szFindTag = pModel.lock()->Get_ModelKey();

					if (ModelTag.end() == find_if(ModelTag.begin(), ModelTag.end(), [&](string _szTag)->_bool { return (_szTag == szFindTag); }))
						ModelTag.push_back(pModel.lock()->Get_ModelKey());
				}

				++iIndex;
			}

			m_pSubGameObjects.clear();
			++iter_elem;

			continue;
		}

		else if (typeid(CEditMapCollider).hash_code() == iter_elem->HashCode)
		{
			++iter_elem;
			continue;
		}

		Out_Json["GameObject"][iIndex]["Name"]				= iter_elem->TypeName;
		Out_Json["GameObject"][iIndex]["Hash"]				= iter_elem->HashCode;
		Out_Json["GameObject"][iIndex]["Setting"]["Enable"] = iter_elem->pInstance.lock()->Get_Enable();
		Out_Json["GameObject"][iIndex]["Component"]["Transform"].emplace();

		iter_elem->pInstance.lock()->Write_Json(Out_Json["GameObject"][iIndex]);

		weak_ptr<CVIBuffer_Model_Instance> pModel = iter_elem->pInstance.lock()->Get_Component<CVIBuffer_Model_Instance>();

		if (pModel.lock().get())
		{
			string szFindTag = pModel.lock()->Get_ModelKey();

			if (ModelTag.end() == find_if(ModelTag.begin(), ModelTag.end(), [&](string _szTag)->_bool { return (_szTag == szFindTag); }))
				ModelTag.push_back(pModel.lock()->Get_ModelKey());
		}

		++iIndex;
		++iter_elem;
	}

	for (auto& elem_group : m_pObjGroup)
	{
		for (auto& elem : elem_group.second)
		{
			Out_Json["GameObject"][iIndex]["Name"]				= elem.TypeName;
			Out_Json["GameObject"][iIndex]["Hash"]				= elem.HashCode;
			Out_Json["GameObject"][iIndex]["Setting"]["Enable"] = elem.pInstance.lock()->Get_Enable();
			Out_Json["GameObject"][iIndex]["Component"]["Transform"].emplace();

			elem.pInstance.lock()->Write_Json(Out_Json["GameObject"][iIndex]);
			++iIndex;
		}
	}

	json Model_json;

	if (!ModelTag.empty())
	{
		string szPath[] =
		{
			"../Bin/Resources/Meshes/Map_Else/Binary/",
			"../Bin/Resources/Meshes/Map_Lv1_Circus/Binary/",
			"../Bin/Resources/Meshes/Map_Lv2_Fortress/Binary/",
			"../Bin/Resources/Meshes/Map_Lv3_Garden/Binary/"
		};

		vector<string> ModelList[4];

		for (_uint i = 0; i < 4; ++i)
		{
			fs::directory_iterator itr(szPath[i]);
			string szFileName;

			while (itr != fs::end(itr))
			{
				const fs::directory_entry& entry = *itr;

				szFileName = entry.path().filename().string();

				if (string::npos != szFileName.find(".bin"))
				{
					ModelList[i].push_back(szFileName.substr(0, szFileName.find(".")));
				}

				++itr;
			}
		}

		_uint iIndex = 0;
		for (auto& iter : ModelTag)
		{
			for (_uint i = 0; i < 4; ++i)
			{
				string szModelTag = iter;

				if (ModelList[i].end() != find_if(ModelList[i].begin(), ModelList[i].end(), [&](string _szTag)->_bool { return (iter == _szTag); }))
				{
					Model_json["ModelTag"][iIndex++] = szPath[i] + szModelTag + ".bin";
					break;
				}
			}
		}
	}

	CJson_Utility::Save_Json("../Bin/ModelLoadList/Temp.json", Model_json);

	if (m_RenderMSG_BOX)
		MSG_BOX("Save_Done");
}

void CWindow_HierarchyView::Write_Json_ModelList()
{
}

void CWindow_HierarchyView::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("GameObject") == In_Json.end())
	{
		return;
	}

	for (auto& Elem_GameObject : In_Json["GameObject"])
	{
		GAMEOBJECT_DESC TempDesc;
		
		TempDesc.TypeName = Elem_GameObject["Name"];
		TempDesc.HashCode = Elem_GameObject["Hash"].get<nlohmann::json::number_unsigned_t>();

		if (typeid(CGround).hash_code() == TempDesc.HashCode)
		{
			TempDesc.TypeName = typeid(CEditGround).name();
			TempDesc.HashCode = typeid(CEditGround).hash_code();
		}

		else if (typeid(CStatic_Instancing_Prop).hash_code() == TempDesc.HashCode)
		{
			TempDesc.TypeName = typeid(CEditInstanceProp).name();
			TempDesc.HashCode = typeid(CEditInstanceProp).hash_code();
		}

		else if (typeid(CPhysXColliderObject).hash_code() == TempDesc.HashCode)
		{
			weak_ptr<CGameObject> pNewGameObject = GAMEINSTANCE->Add_GameObject(TempDesc.HashCode, LEVEL::LEVEL_EDIT);
			pNewGameObject.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);
			pNewGameObject.lock()->Load_FromJson(Elem_GameObject);

			auto iter_find = m_pObjGroup.find(TempDesc.HashCode);

			if (iter_find == m_pObjGroup.end())
			{
				vector<GAMEOBJECT_DESC> List;
				List.push_back({ TempDesc.HashCode, TempDesc.TypeName, pNewGameObject });

				m_pObjGroup[TempDesc.HashCode] = List;
			}
			else
			{
				iter_find->second.push_back({ TempDesc.HashCode, TempDesc.TypeName, pNewGameObject });
			}

			continue;
		}

		else if (typeid(CEditMapCollider).hash_code() == TempDesc.HashCode)
			continue;

		Call_Add_GameObject_Internal(TempDesc.HashCode, TempDesc.TypeName.c_str());
		m_pGameObjects.back().pInstance.lock()->Set_Enable(Elem_GameObject["Setting"]["Enable"]);
		m_pGameObjects.back().pInstance.lock()->Load_FromJson(Elem_GameObject);
	}

	if (m_pObjGroup.end() != m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code()))
	{
		GAMEOBJECT_DESC TempDesc;
		TempDesc.HashCode = typeid(CEditMapCollider).hash_code();
		TempDesc.TypeName = typeid(CEditMapCollider).name();

		Call_Add_GameObject_Internal(TempDesc.HashCode, TempDesc.TypeName.c_str());
		m_pGameObjects.back().pInstance.lock()->Set_Enable(true);
	}
}

void CWindow_HierarchyView::Call_Add_GameObject_Internal(const _hashcode& TypeHash, const char* TypeName)
{
	void* pArg = nullptr;

	if (typeid(CCamera_Free).hash_code() == TypeHash)
	{
		CCamera::CAMERADESC			CameraDesc;
		ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
		CameraDesc.vEye		= _float4(0.0f, 10.f, -10.f, 1.f);
		CameraDesc.vAt		= _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.fFovy	= XMConvertToRadians(65.0f);
		CameraDesc.fAspect	= (_float)g_iWinCX / g_iWinCY;
		CameraDesc.fNear	= 0.2f;
		CameraDesc.fFar		= 300.f;

		pArg = &CameraDesc;
	}

	weak_ptr<CGameObject> pNewGameObject = GAMEINSTANCE->Add_GameObject(TypeHash, LEVEL::LEVEL_EDIT, pArg);
	pNewGameObject.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);

	m_pGameObjects.push_back({ TypeHash, TypeName, pNewGameObject });
}

void CWindow_HierarchyView::Picking_Obj()
{
	if (KEY_INPUT(KEY::ALT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		_int iIndex						= 0;
		_int iPickedIndex				= -1;
		_float fDist					= 99999.f;
		GAMEOBJECT_DESC* pPickingObj	= nullptr;

		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		for (auto& elem : m_pGameObjects)
		{
			if (elem.pInstance.lock()->IsPicking(MouseRayInWorldSpace, fDist))
			{
				iPickedIndex = iIndex;
				pPickingObj  = &elem;
			}	
			
			++iIndex;
		}

		if (0 <= iPickedIndex)
		{
			CallBack_ListClick(*pPickingObj);

			m_pGameObjects[m_iPreSelectIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_NONE);
			pPickingObj->pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_ACCEPT);

			m_iPreSelectIndex = iPickedIndex;
		}
	}
}

void CWindow_HierarchyView::OnLevelLoad()
{
	for (auto& elem : m_pGameObjects)
	{
		if (elem.pInstance.lock())
			elem.pInstance.lock()->Set_Dead();
	}

	m_pGameObjects.clear();
}

void CWindow_HierarchyView::Free()
{
	m_RenderMSG_BOX = false;

	json NewJson;
	Write_Json(NewJson);
	
	if (!NewJson.empty())
	{
		time_t timer     = time(NULL);
		tm     TimeDesc;
		
		localtime_s(&TimeDesc, &timer);

		string szPath
			= string("../Bin/LevelData/AutoSave/AutoSave ") 
			+ to_string(TimeDesc.tm_mon)  + "."
			+ to_string(TimeDesc.tm_wday) + " ("
			+ to_string(TimeDesc.tm_hour) + "-"
			+ to_string(TimeDesc.tm_min)  + "-"
			+ to_string(TimeDesc.tm_sec)  + ").json";

		CJson_Utility::Save_Json(szPath.c_str(), NewJson);
	}

	for (auto& elem : m_pGameObjects)
	{
		if(elem.pInstance.lock())
			elem.pInstance.lock()->Set_Dead();
	}

	m_pGameObjects.clear();
}

