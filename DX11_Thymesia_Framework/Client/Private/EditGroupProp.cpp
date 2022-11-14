#include "stdafx.h"

#include "EditGroupProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"

#include "Prop.h"


#include <Direct.h>

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

	Load_ResourceList(m_AnimList   , "../Bin/MapTool_MeshInfo/ANIM/", ".bin");
	Load_ResourceList(m_NonAnimList, "../Bin/GROUND/ANIM/", ".bin");
	Load_ResourceList(m_JsonList   , "../Bin/MapTool_MeshInfo/ANIM/", ".json");
	 
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
		if (ImGui::BeginTabItem("Edit"))
		{
			View_EditMode();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("List"))
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

void CEditGroupProp::View_CrateProp()
{
	static const char* ModelType[] =
	{
		"NONANIM",
		"ANIM"
	};

	static _int		iSelect_ModelType		= 0;
	static _int		iSelect_NonAnimModel	= 0;
	static _int		iSelect_AnimModel		= 0;
	static string	szSelect_ModelTag		= "";

	if (ImGui::Combo("Type", &iSelect_ModelType, ModelType, IM_ARRAYSIZE(ModelType)))
	{
		if (0 == iSelect_ModelType)
			szSelect_ModelTag = m_NonAnimList[iSelect_NonAnimModel];

		else if (1 == iSelect_ModelType)
			szSelect_ModelTag = m_NonAnimList[iSelect_AnimModel];
	}

	ImGui::Text(string("[ Select MD ] : " + szSelect_ModelTag).c_str());

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

					if (ImGui::Selectable(iter.c_str(), is_selected))
					{
						iSelect_NonAnimModel = iIndex;
						szSelect_ModelTag    = iter;
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

					if (ImGui::Selectable(iter.c_str(), is_selected))
					{
						iSelect_AnimModel = iIndex;
						szSelect_ModelTag = iter;
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

}

void CEditGroupProp::View_PickingInfo()
{
	string szPickingTag
		= string("[ Info ] : ")
		+ "( " + to_string(m_iPickingIndex) + " )";

	szPickingTag += (0 > m_iPickingIndex || (_int)m_PropList.size() <= m_iPickingIndex)
		? (" None ") 
		: (m_PropList[m_iPickingIndex].szName);

	ImGui::Text(szPickingTag.c_str());
}

void CEditGroupProp::View_ModelList()
{
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