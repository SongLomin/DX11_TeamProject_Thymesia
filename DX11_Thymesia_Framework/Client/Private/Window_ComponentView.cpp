#include "stdafx.h"
#include "Window_ComponentView.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "Client_Components.h"
#include "Window_HierarchyView.h"

#define PICKED_GAMEOBJECT m_tPickedGameObjectDesc.pInstance.lock().get()

IMPLEMENT_SINGLETON(CWindow_ComponentView)

HRESULT CWindow_ComponentView::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC("Component_View", ImVec2(300.f, 500.f), window_flags);

	return S_OK;
}

void CWindow_ComponentView::Start()
{
	GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick += bind(&CWindow_ComponentView::Call_Click_GameObject, this, placeholders::_1);

}


void CWindow_ComponentView::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



}

HRESULT CWindow_ComponentView::Render()
{


	if (FAILED(__super::Begin()))
		return E_FAIL;

	//ImGui::Text("UI_TOOL");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (PICKED_GAMEOBJECT)
	{
		Draw_Components();
	}

	//for (auto& elem : m_pGameObjects)
	//{
	//	if (ImGui::Selectable(elem.TypeName))
	//	{
	//		//CallBack_ListClick(elem.HashCode, elem.TypeName);
	//	}
	//}



	__super::End();

	return S_OK;
}

void CWindow_ComponentView::Call_Click_GameObject(GAMEOBJECT_DESC GameObjectDesc)
{
	m_tPickedGameObjectDesc = GameObjectDesc;
	Init_Components();

}

void CWindow_ComponentView::Draw_Components()
{
	if (!PICKED_GAMEOBJECT)
		return;

	PICKED_GAMEOBJECT->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW);

	if (ImGui::CollapsingHeader("GameObject Setting"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::Button("Delete Object"))
		{
			PICKED_GAMEOBJECT->Set_Dead();
		}


		_bool bEnable = PICKED_GAMEOBJECT->Get_Enable();

		ImGui::Text("Enable");
		ImGui::Checkbox("Enable", &bEnable);
		ImGui::Separator();

		PICKED_GAMEOBJECT->Set_Enable(bEnable);
	}

	weak_ptr<CUI> pCastUI = Weak_Cast<CUI>(m_tPickedGameObjectDesc.pInstance);

	if (pCastUI.lock())
		return;

	weak_ptr<CTransform> pTransformCom = PICKED_GAMEOBJECT->Get_Component<CTransform>();

	if (pTransformCom.lock().get())
	{
		if (ImGui::CollapsingHeader("Transform Component"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			{ // Position
				_vector vPositionVector = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

				ImGui::Text("Position");
				ImGui::DragFloat3("##Position", &vPositionVector.m128_f32[0], 1.f);
				ImGui::Separator();

				pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPositionVector);
			}
			
			{ // Quaternion
				_matrix matWorld = pTransformCom.lock()->Get_WorldMatrix();
				_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matWorld));
				//_vector vQuaternion = XMQuaternionRotationMatrix();

				ImGui::Text("Pitch Yaw Roll");
				ImGui::DragFloat3("##Pitch Yaw Roll", &vPitchYawRoll.x, 0.01f);
				ImGui::Separator();

				_vector vQuaternion = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

				pTransformCom.lock()->Rotation_Quaternion(vQuaternion);
			}
			
			{ // Scale
				_float3 vScaleFloat3 = pTransformCom.lock()->Get_Scaled();
				_vector vScaleVector = XMLoadFloat3(&vScaleFloat3);

				ImGui::Text("Scale");
				ImGui::DragFloat3("##Scale", &vScaleVector.m128_f32[0], 0.1f);
				ImGui::Separator();

				vScaleVector.m128_f32[0] = max(0.00001f, vScaleVector.m128_f32[0]);
				vScaleVector.m128_f32[1] = max(0.00001f, vScaleVector.m128_f32[1]);
				vScaleVector.m128_f32[2] = max(0.00001f, vScaleVector.m128_f32[2]);

				XMStoreFloat3(&vScaleFloat3, vScaleVector);

				pTransformCom.lock()->Set_Scaled(vScaleFloat3);
			}
		}
	}

	weak_ptr<CModel> pModel = PICKED_GAMEOBJECT->Get_Component<CModel>();

	if (pModel.lock().get())
	{
		if (ImGui::CollapsingHeader("CModel Component"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			//ImGui::Text("Input Model Key");
			//ImGui::InputText("##ModelKey", m_szModelKey, MAX_PATH);

			ImGui::Text("Model List");
			if (ImGui::BeginListBox("##Model List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{

				for (int i = 0; i < m_AllModelKeys.size(); i++)
				{
					const bool is_selected = (m_CurrentModelIndex == i);
					if (ImGui::Selectable(m_AllModelKeys[i].c_str(), is_selected))
						m_CurrentModelIndex = i;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			if (ImGui::Button("Load"))
			{
				pModel.lock()->Init_Model(m_AllModelKeys[m_CurrentModelIndex].c_str());
			}
		}
	}

}

void CWindow_ComponentView::Init_Components()
{

	weak_ptr<CModel> pModel = PICKED_GAMEOBJECT->Get_Component<CModel>();

	if (pModel.lock())
	{
		//strcpy_s(m_szModelKey, pModel.lock()->Get_ModelKey());
		
		const char* szCurrentModelKey = pModel.lock()->Get_ModelKey();

		m_AllModelKeys = GAMEINSTANCE->Get_AllModelKeys();
		auto iter =	 find_if(m_AllModelKeys.begin(), m_AllModelKeys.end(), [&](string& ModelKey)
			{
				return !strcmp(szCurrentModelKey, ModelKey.c_str());
			});

		m_CurrentModelIndex = iter - m_AllModelKeys.begin();
	}
}


void CWindow_ComponentView::Free()
{
}



