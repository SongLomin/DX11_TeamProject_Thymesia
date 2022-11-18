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

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (PICKED_GAMEOBJECT)
	{
		Draw_Components();
	}

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
	static _bool bSelect_ActivateHotkey  = false;
	static _bool bSelect_ActivatePicking = false;

	if (pTransformCom.lock().get() && typeid(CEditGroupProp).hash_code() != m_tPickedGameObjectDesc.HashCode)
	{
		if (bSelect_ActivatePicking)
			Picking_Obj();

		if (bSelect_ActivateHotkey)
			TransformComponent_PickingAction(pTransformCom);

		if (ImGui::CollapsingHeader("Transform Component"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Checkbox("Trasnfrom HotKey", &bSelect_ActivateHotkey);
			ImGui::Checkbox("Acting Picking", &bSelect_ActivatePicking);

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

	static _char    szFindModelTag[MAX_PATH] = "";

	if (pModel.lock().get())
	{
		if (ImGui::CollapsingHeader("CModel Component"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("Model List");

			ImGui::Text("");
			ImGui::InputText("Model Tag Find", szFindModelTag, MAX_PATH);
			ImGui::Text("");

			if (ImGui::BeginListBox("##Model List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (int i = 0; i < m_AllModelKeys.size(); i++)
				{
					const _bool is_selected = (m_CurrentModelIndex == i);

					if (0 < strlen(szFindModelTag))
					{
						if (string::npos == m_AllModelKeys[i].find(szFindModelTag))
							continue;
					}

					if (ImGui::Selectable(m_AllModelKeys[i].c_str(), is_selected))
						m_CurrentModelIndex = i;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			if (ImGui::Button("Load"))
			{
				if (!pModel.lock())
					return;

				pModel.lock()->Init_Model(m_AllModelKeys[m_CurrentModelIndex].c_str());
			}
		}
	}

}

void CWindow_ComponentView::Init_Components()
{
	vector<string> ModelKeys = GET_SINGLE(CGameInstance)->Get_AllNoneAnimModelKeys();

	for (auto& iter : ModelKeys)
	{
		const char* pKey = iter.c_str();

		auto iter = find_if(m_AllModelKeys.begin(), m_AllModelKeys.end(), [&](string& ModelKey)
		{
			return !strcmp(pKey, ModelKey.c_str());
		});

		if (iter != m_AllModelKeys.end())
			continue;

		m_AllModelKeys.push_back(pKey);
	}

	weak_ptr<CModel> pModel = PICKED_GAMEOBJECT->Get_Component<CModel>();

	if (pModel.lock())
	{
		if (pModel.lock()->Get_ModelData().lock())
		{
			const char* szCurrentModelKey = pModel.lock()->Get_ModelKey();

			m_AllModelKeys = GAMEINSTANCE->Get_AllModelKeys();

			auto iter = find_if(m_AllModelKeys.begin(), m_AllModelKeys.end(), [&](string& ModelKey)
				{
					return !strcmp(szCurrentModelKey, ModelKey.c_str());
				});

			m_CurrentModelIndex = iter - m_AllModelKeys.begin();
		}
	}
}

void CWindow_ComponentView::Picking_Obj()
{
	if (KEY_INPUT(KEY::ALT, KEY_STATE::HOLD) && KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		for (auto& iter : GET_SINGLE(CWindow_HierarchyView)->m_pGameObjects)
		{
			weak_ptr<CModel>		pModelCom     = iter.pInstance.lock()->Get_Component<CModel>();
			weak_ptr<CTransform>	pTransformCom = iter.pInstance.lock()->Get_Component<CTransform>();

			if (!pModelCom.lock() || !pTransformCom.lock())
				continue;

			MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, pTransformCom.lock()->Get_WorldMatrix()))
			{
				GET_SINGLE(CWindow_HierarchyView)->CallBack_ListClick(iter);
			}
		}
	}
}

void CWindow_ComponentView::TransformComponent_PickingAction(weak_ptr<CTransform> _pTransform)
{
	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);
	_float4 vMouseDir;
	ZeroMemory(&vMouseDir, sizeof(_float4));

	_bool _bClick_Terrain = SMath::Is_Picked(MouseRayInWorldSpace, &vMouseDir);

	// Z : 이동, X : 로테이션, 마우스 휠 : y축 이동

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (_bClick_Terrain && KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			_vector vObjPos = _pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vAddPos = XMVectorSet(vMouseDir.x, vObjPos.m128_f32[1], vMouseDir.z, 1.f);

			_pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vAddPos);
		}

		else if (_bClick_Terrain && KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_vector vObjPos = _pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vAddPos = XMVectorSet(vMouseDir.x, vObjPos.m128_f32[1], vMouseDir.z, 1.f);

			_pTransform.lock()->LookAt(vAddPos);
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
				_vector vObjPos = _pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
				vObjPos.m128_f32[1] += (MouseMove * -0.01f);

				_pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vObjPos);
			}
		}
	}
}

void CWindow_ComponentView::Free()
{
}



