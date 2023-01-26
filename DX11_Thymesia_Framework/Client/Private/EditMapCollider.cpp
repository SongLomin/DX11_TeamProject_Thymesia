#include "stdafx.h"

#include "EditMapCollider.h"

#include "PhysXColliderObject.h"
#include "Window_HierarchyView.h"
#include "imgui.h"
#include "SMath.h"                                                            

GAMECLASS_C(CEditMapCollider)
CLONE_C(CEditMapCollider, CGameObject)

HRESULT CEditMapCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditMapCollider::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	XMStoreFloat4x4(&m_PickingDesc, XMMatrixIdentity());

    return S_OK;
}

HRESULT CEditMapCollider::Start()
{
    return S_OK;
}

void CEditMapCollider::Tick(_float fTimeDelta)
{
}

void CEditMapCollider::LateTick(_float fTimeDelta)
{
}

HRESULT CEditMapCollider::Render(ID3D11DeviceContext* pDeviceContext)
{
    return S_OK;
}

void CEditMapCollider::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDITDRAW:
        {
            if (ImGui::BeginTabBar("Edit"))
            {
                if (ImGui::BeginTabItem("Create"))
                {
                    View_Picking_Collider();
					View_Picking_List();
					View_SelectTransformInfo();
					View_Picking_Option();

					ImGui::EndTabItem();
                }

				if (ImGui::BeginTabItem("Act"))
				{
					View_Act_DeleteSame();

					ImGui::EndTabItem();
				}

                ImGui::EndTabBar();
            }
        }
        break;

		case EVENT_TYPE::ON_EDIT_PHYSXINFO:
		{
			auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

			if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
				return;

			for (auto& iter : iter_collider->second)
				iter.pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO);

			m_bRender = true;
		}
		break;

		case EVENT_TYPE::ON_EDIT_PHYSXINFO_N:
		{
			auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

			if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
				return;

			for (auto& iter : iter_collider->second)
				iter.pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO_N);

			m_bRender = false;
		}
		break;

		case EVENT_TYPE::ON_EDIT_DELETE:
		{
			auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

			if (iter_collider != GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
			{
				for (auto& elem : iter_collider->second)
					elem.pInstance.lock()->Set_Dead();

				iter_collider->second.clear();
			}
		}
		break;
    }
}

void CEditMapCollider::Write_Json(json& Out_Json)
{
}

void CEditMapCollider::View_Picking_Collider()
{
    if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
        return;

	// 생성
	_int iPreIndex = m_iPickingIndex;

	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		_float4 vOutPos;
		ZeroMemory(&vOutPos, sizeof(_float4));
		vOutPos.y = m_PickingDesc._42;

		if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vOutPos))
			return;

		vOutPos.y = m_PickingDesc._42;
		memcpy(&m_PickingDesc.m[3], &vOutPos, sizeof(_float3));

		weak_ptr<CPhysXColliderObject> pObj = GAMEINSTANCE->Add_GameObject<CPhysXColliderObject>(LEVEL::LEVEL_EDIT);
		pObj.lock()->Get_Component<CTransform>().lock()->Set_WorldMatrix(XMLoadFloat4x4(&m_PickingDesc));

		auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

		if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		{
			CImGui_Window::GAMEOBJECT_DESC Desc;
			Desc.HashCode	= typeid(CPhysXColliderObject).hash_code();
			Desc.pInstance	= pObj;
			Desc.TypeName	= typeid(CPhysXColliderObject).name();

			vector<CImGui_Window::GAMEOBJECT_DESC> List;
			List.push_back(Desc);

			GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.emplace(Desc.HashCode, List);

			m_iPickingIndex = 0;
		}
		else
		{
			CImGui_Window::GAMEOBJECT_DESC Desc;
			Desc.HashCode	= typeid(CPhysXColliderObject).hash_code();
			Desc.pInstance	= pObj;
			Desc.TypeName	= typeid(CPhysXColliderObject).name();

			iter_collider->second.push_back(Desc);

			m_iPickingIndex = _uint((_uint)iter_collider->second.size() - 1);
		}

		if (m_bRender)
			pObj.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO);
	}

	// 피킹
	else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		MESH_VTX_INFO VtxInfo;
		VtxInfo.vMax = {  0.5f,  0.5f,  0.5f };
		VtxInfo.vMin = { -0.5f, -0.5f, -0.5f };

		_float fPickedDist;
		_uint   iIndex			= 0;
		_float  fDistance		= 99999999.f;
		_float4	vCamPosition	= GAMEINSTANCE->Get_CamPosition();
		_vector vCamPos			= XMLoadFloat4(&vCamPosition);

		auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

		if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
			return;

		for (auto& iter : iter_collider->second)
		{
			weak_ptr<CTransform> pTransform = iter.pInstance.lock()->Get_Component<CTransform>();

			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, pTransform.lock()->Get_WorldMatrix()))
			{
				_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION)));

				if (fLength < fDistance)
				{
					fDistance		= fLength;
					m_iPickingIndex	= iIndex;
					XMStoreFloat4x4(&m_PickingDesc, pTransform.lock()->Get_WorldMatrix());
				}
			}

			++iIndex;
		}
	}

	if ((0 <= iPreIndex) && (iPreIndex != m_iPickingIndex))
	{
		auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

		if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
			return;

		_int iSize = (_int)iter_collider->second.size();

		if (iSize > iPreIndex)
			iter_collider->second[iPreIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_NONE);

		if (iSize > m_iPickingIndex)
			iter_collider->second[m_iPickingIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW_ACCEPT);
	}
}
void CEditMapCollider::View_Picking_List()
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	ImGui::Text(string(string(" Size : ") + to_string((_uint)iter_collider->second.size())).c_str());

	if (ImGui::TreeNode("[ Show List ]"))
	{
		if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (_uint i = 0; i < (_uint)iter_collider->second.size(); ++i)
			{
				const bool is_selected = (m_iPickingIndex == i);

				string szTag = "( " + to_string(i) + " ) " + string("  Coliider");

				if (ImGui::Selectable(szTag.c_str(), is_selected))
				{
					m_iPickingIndex = i;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndListBox();
		}

		ImGui::TreePop();
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete", ImVec2(100.f, 25.f)))
	{
		if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
			return;

		auto iter = iter_collider->second.begin() + m_iPickingIndex;

		if (iter_collider->second.end() != iter)
		{
			iter->pInstance.lock()->Set_Dead();
			iter_collider->second.erase(iter);
		}
	}
}
void CEditMapCollider::View_SelectTransformInfo()
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return;

	if (iter_collider->second.size() <= m_iPickingIndex)
		return;

	weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();

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

	XMStoreFloat4x4(&m_PickingDesc, pTransformCom.lock()->Get_WorldMatrix());
}
void CEditMapCollider::View_Picking_Option()
{
	if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
		m_iOption = 0;
	if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
		m_iOption = 1;
	if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
		m_iOption = 2;

	static const char* szOptionTag[] =
	{
		"<< Act X >>",
		"<< Act Y >>",
		"<< Act Z >>"
	};

	ImGui::Text("");
	ImGui::Text(string("Select Option : " + string(szOptionTag[m_iOption])).c_str());
	ImGui::Text(string("Pick Index : " + to_string(m_iPickingIndex)).c_str());

	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return;

	if (iter_collider->second.size() <= m_iPickingIndex)
		return;

	// Z : 터레인 이동
	// X : 회전(옵션 활성화)
	// C : 수동 이동(옵션 활성화)
	// R : 삭제

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);
			_float4 vMouseDir;

			vMouseDir.y = m_PickingDesc._42;

			if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vMouseDir))
				return;

			iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>().lock()->Set_State
			(
				CTransform::STATE_TRANSLATION,
				XMVectorSet(vMouseDir.x, m_PickingDesc._42, vMouseDir.z, 1.f)
			);
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
				weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();
				_vector vPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

				vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + (-0.01f * MouseMove));
				pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
		}

		else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
			{
				weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();
				_matrix matWorld      = pTransformCom.lock()->Get_WorldMatrix();
				_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matWorld));

				switch (m_iOption)
				{
					case 0: vPitchYawRoll.x += 0.01f * MouseMove; break;
					case 1: vPitchYawRoll.y += 0.01f * MouseMove; break;
					case 2: vPitchYawRoll.z += 0.01f * MouseMove; break;
				}

				_vector vQuaternion = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

				pTransformCom.lock()->Rotation_Quaternion(vQuaternion);
			}
		}
	}
	else
	{
		if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::R, KEY_STATE::TAP))
		{
			auto iter = iter_collider->second.begin() + m_iPickingIndex;

			if (iter_collider->second.end() != iter)
			{
				iter->pInstance.lock()->Set_Dead();
				iter_collider->second.erase(iter);
			}
		}

		else if (KEY_INPUT(KEY::V, KEY_STATE::HOLD))
		{
			weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();

			_float3 vScaleFloat3 = pTransformCom.lock()->Get_Scaled();

			switch (m_iOption)
			{
				case 0:
				{
					if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
						vScaleFloat3.x -= 0.1f;
					else if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
						vScaleFloat3.x += 0.1f;
				}
				break;

				case 1:
				{
					if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
						vScaleFloat3.y -= 0.1f;
					else if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
						vScaleFloat3.y += 0.1f;
				}
				break;

				case 2:
				{
					if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
						vScaleFloat3.z -= 0.1f;
					else if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
						vScaleFloat3.z += 0.1f;
				}
				break;
			}

			pTransformCom.lock()->Set_Scaled(vScaleFloat3);
		}

		else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::H, KEY_STATE::HOLD))
		{
			if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
				return;

			if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
				return;

			if (iter_collider->second.size() <= m_iPickingIndex)
				return;

			weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();

			if (KEY_INPUT(KEY::X, KEY_STATE::TAP))
			{
				pTransformCom.lock()->Rotation_Quaternion(XMVectorSet(0.f, 0.f, 0.f, 0.f));
			}

			else if (KEY_INPUT(KEY::C, KEY_STATE::TAP))
			{
				pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			}

			else if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
			{
				pTransformCom.lock()->Set_Scaled(_float3(1.f, 1.f, 1.f));
			}
		}
	}
}

void CEditMapCollider::View_Act_DeleteSame()
{
	if (!ImGui::Button("Act Delete", ImVec2(100.f, 25.f)))
		return;

	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CPhysXColliderObject).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	for (auto& elem : iter_collider->second)
	{
		for (auto& elem_compare : iter_collider->second)
		{
			if (elem.pInstance.lock() == elem_compare.pInstance.lock())
				continue;

			if (elem.pInstance.lock()->Get_Dead() || elem_compare.pInstance.lock()->Get_Dead())
				continue;

			weak_ptr<CTransform> pTransform			= elem.pInstance.lock()->Get_Component<CTransform>();
			weak_ptr<CTransform> pTransform_compare = elem_compare.pInstance.lock()->Get_Component<CTransform>();

			_matrix Mat			= pTransform.lock()->Get_WorldMatrix();
			_matrix Mat_compare	= pTransform_compare.lock()->Get_WorldMatrix();

			_bool bDelete = true;
			for (_uint i = 0; i < 4; ++i)
			{
				bDelete &= XMVector3Equal(Mat.r[i], Mat_compare.r[i]);
			}
			
			if (bDelete)
				elem_compare.pInstance.lock()->Set_Dead();
		}
	}
}

void CEditMapCollider::Free()
{
}
