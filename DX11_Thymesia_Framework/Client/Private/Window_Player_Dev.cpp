#include "stdafx.h"
#include "Window_Player_Dev.h"
#include "GameInstance.h"
#include "GameManager.h"	
#include "Camera_Target.h"
#include "PhysXCharacterController.h"
#include "MeshContainer.h"

IMPLEMENT_SINGLETON(CWindow_Player_Dev)

HRESULT CWindow_Player_Dev::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	SetUp_ImGuiDESC("Player_Option", ImVec2(300.f, 450.f), window_flags);

	return S_OK;
}

void CWindow_Player_Dev::Start()
{
}

void CWindow_Player_Dev::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	

	

}

void CWindow_Player_Dev::Background_Tick(_float fTimeDelta)
{
	__super::Background_Tick(fTimeDelta);




}

HRESULT CWindow_Player_Dev::Render(ID3D11DeviceContext* pDeviceContext)
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
	{
		__super::End();
		return S_OK;
	}

	if (ImGui::CollapsingHeader("Position"))
	{
		ImGui::DragFloat3("##Player Position", &m_vPlayerPosition.x);

		if (ImGui::Button("Set Position"))
		{
			Set_PlayerPosition(m_vPlayerPosition);
		}

		if (ImGui::Button("Get Position"))
		{
			XMStoreFloat3(&m_vPlayerPosition, Get_PlayerPosition());
		}

		if (ImGui::Button("Stage3. Varg Zone"))
		{
			Set_PlayerPosition(_float3(75.f, 0.f, 40.f));
		}
	}

	if (ImGui::CollapsingHeader("NvCloth"))
	{
		weak_ptr<CPlayer> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

		if (pPlayer.lock())
		{
			weak_ptr<CMeshContainer> pCapeMeshContainer = pPlayer.lock()->Get_Component<CModel>().lock()->Get_MeshContainer(2);

			::vector<_float>& PlayerCapeInvMess = pCapeMeshContainer.lock()->Get_InvMesses();

			string szDefaultName = "Index ";
			string szIndexName;

			if (ImGui::Button("Update InvMess"))
			{
				pCapeMeshContainer.lock()->Update_InvMesses();
			}

			for (_size_t i = 0; i < PlayerCapeInvMess.size(); ++i)
			{
				szIndexName = szDefaultName + to_string(i);
				ImGui::DragFloat(szIndexName.c_str(), &PlayerCapeInvMess[i], 0.05f, 0.f, 1.f, "%.3f", 0);
			}
		}
	}

	

	//std::system_category().message(hr)
	__super::End();

	return S_OK;
}

void CWindow_Player_Dev::Write_Json(json& Out_Json)
{

}

void CWindow_Player_Dev::Load_FromJson(const json& In_Json)
{

}

void CWindow_Player_Dev::Set_PlayerPosition(const _float3& In_vPosition)
{
	weak_ptr<CPlayer> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (pPlayer.lock())
	{
		PxControllerFilters Filters;

		pPlayer.lock()->Get_Component<CPhysXCharacterController>().lock()->Set_Position(XMLoadFloat3(&In_vPosition), GAMEINSTANCE->Get_DeltaTime(), Filters);
	}
}

_vector CWindow_Player_Dev::Get_PlayerPosition()
{
	weak_ptr<CPlayer> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (pPlayer.lock())
	{
		return pPlayer.lock()->Get_Component<CPhysXCharacterController>().lock()->Get_Position();
	}

	return {};
}

void CWindow_Player_Dev::Set_PlayerGravity(const _bool In_bGravity)
{
	weak_ptr<CPlayer> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (pPlayer.lock())
	{
		pPlayer.lock()->Get_Component<CPhysXCharacterController>().lock()->Enable_Gravity(In_bGravity);
	}
}

void CWindow_Player_Dev::Free()
{

}