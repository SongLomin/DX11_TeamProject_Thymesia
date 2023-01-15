#include "stdafx.h"

#include "EditSetActor.h"
#include "Client_GameObjects.h"

#include "Shader.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "Model.h"
#include "Renderer.h"

#include "Monster.h"

#include "PhysXColliderObject.h"
#include "Window_HierarchyView.h"
#include "SMath.h"
#include "imgui.h"

GAMECLASS_C(CEditSetActor)
CLONE_C(CEditSetActor, CGameObject)

HRESULT CEditSetActor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditSetActor::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	XMStoreFloat4x4(&m_PickingDesc, XMMatrixIdentity());

	m_pSelect_ShaderCom = Add_Component<CShader>();
	m_pSelect_VIBufferCom = Add_Component<CVIBuffer_DynamicCube>();

	m_pSelect_ShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxCubeTex"),
		VTXCUBETEX_DECLARATION::Element,
		VTXCUBETEX_DECLARATION::iNumElements
	);

	return S_OK;
}

HRESULT CEditSetActor::Start()
{
	return S_OK;
}

void CEditSetActor::Tick(_float fTimeDelta)
{
}

void CEditSetActor::LateTick(_float fTimeDelta)
{
	if (!m_bSubDraw)
		return;

	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Cast<CGameObject>(m_this));
}

HRESULT CEditSetActor::Render(ID3D11DeviceContext* pDeviceContext)
{
	SetUp_ShaderResource_Select(pDeviceContext);

	return S_OK;
}

void CEditSetActor::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	switch ((EVENT_TYPE)iArg)
	{
		case EVENT_TYPE::ON_EDITDRAW_ACCEPT:
		{
			m_bSubDraw = true;
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW_NONE:
		{
			m_bSubDraw = false;
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW:
		{
			if (!m_bSubDraw)
				return;

			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{
					View_CreateActor();
					View_Picking_Actor();
					View_Picking_List();
					View_SelectTransformInfo();
					View_Picking_Option();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Select"))
				{
					View_Picking_Actor();
					View_Picking_List();
					View_SelectTransformInfo();
					View_Picking_Option();
					View_Picking_MessageEdit();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;

		case EVENT_TYPE::ON_EDIT_DELETE:
		{
			auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

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

void CEditSetActor::Write_Json(json& Out_Json)
{

}

void CEditSetActor::View_CreateActor()
{
	static const char* ActorTypeList[] =
	{
		"Monster",
		"Elite",
		"Boss",
		"Player"
	};

	static const char* ActorList_Nor[] =
	{
		"Axe Man",						// 1
		"Knife Woman",					// 2
		"Skull",						// 3
		"Gardener",						// 4
		"Shield Axe Man",				// 5
		"Balloon",						// 6
		"Enhance_Gardener",				// 7
		"Skull_Shield",					// 8
		"Skull_Spear",					// 9
		"Armor_Shield",					// 10
		"WeakArmor_Shield",				// 11
		"Armor_Spearman",				// 12
		"WeakArmor_Spearman"			// 13
	};

	static const char* ActorList_Elite[] =
	{
		"Joker",
		"BigHandman"
	};


	static const char* ActorList_Boss[] =
	{
		"Varg",
		"Bat" ,
		"Urd"
	};

	static const char* MonsterActionList[] =
	{
		"Idle",
		"Sit",
		"Fidget",
		"Spidle",
		"RunAttackIdle",
		"SupriseAttack"
	};

	static const char* BossActionList[] =
	{
		"BEGIN START",
		"NORMAL START"
	};

	static _int		iSelect_ActorTypeList = 0;
	static _int		iSelect_ActorList = 0;
	static _int		iSelect_MonsterActionList = 0;
	static _int		iSelect_BossActionList = 0;
	static _int		iSelect_MonsterSection = 0;
	static _bool	bRenderActor = false;
	static _bool	bPatrol = false;
	_bool			bNorMonsterCreate = true;


	if (ImGui::Checkbox("Render Actor", &bRenderActor))
	{
		auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

		if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
			return;

		for (auto& elem : iter_collider->second)
			elem.pInstance.lock()->Set_Enable(bRenderActor);
	}

	if (ImGui::Combo("Monster Type", &iSelect_ActorTypeList, ActorTypeList, IM_ARRAYSIZE(ActorTypeList)))
	{
		iSelect_ActorList = 0;
		iSelect_MonsterActionList = 0;
		iSelect_BossActionList = 0;
	}

	if (1 == iSelect_ActorTypeList)
		ImGui::Combo("Monster Name", &iSelect_ActorList, ActorList_Elite, IM_ARRAYSIZE(ActorList_Elite));
	else if (2 == iSelect_ActorTypeList)
		ImGui::Combo("Monster Name", &iSelect_ActorList, ActorList_Boss, IM_ARRAYSIZE(ActorList_Boss));
	else if (3 != iSelect_ActorTypeList)
		ImGui::Combo("Monster Name", &iSelect_ActorList, ActorList_Nor, IM_ARRAYSIZE(ActorList_Nor));

	if (2 == iSelect_ActorTypeList)
	{
		ImGui::Combo("Boss State", &iSelect_BossActionList, BossActionList, IM_ARRAYSIZE(BossActionList));
		bNorMonsterCreate = false;
	}
	else if (3 != iSelect_ActorTypeList)
	{
		ImGui::Combo("Monster State", &iSelect_MonsterActionList, MonsterActionList, IM_ARRAYSIZE(MonsterActionList));
		bNorMonsterCreate = true;
	}

	ImGui::InputInt("Section Index", &iSelect_MonsterSection);
	ImGui::Checkbox("Patrol", &bPatrol);

	RAY MouseRayInWorldSpace;

	if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Picking(MouseRayInWorldSpace, m_PickingDesc._42))
		return;

	CMonster::STATE_LINK_MONSTER_DESC tMonsterDesc;
	ZeroMemory(&tMonsterDesc, sizeof(CMonster::STATE_LINK_MONSTER_DESC));

	tMonsterDesc.eBossStartType = (!bNorMonsterCreate) ? ((BOSSSTARTTYPE)iSelect_BossActionList) : (BOSSSTARTTYPE::BOSSSTARTEND);
	tMonsterDesc.eNorMonIdleType = (bNorMonsterCreate) ? ((NORMONSTERIDLETYPE)iSelect_MonsterActionList) : (NORMONSTERIDLETYPE::IDLEEND);
	tMonsterDesc.eMonType = (MONSTERTYPE)((iSelect_ActorList + iSelect_ActorTypeList * (_int)MONSTERTYPE::START_ELITE_MONSTER) + 1);
	tMonsterDesc.iSectionIndex = iSelect_MonsterSection;
	tMonsterDesc.bPatrol = bPatrol;
	memcpy(&tMonsterDesc.m_fStartPositon, m_PickingDesc.m[3], sizeof(_float4));

	switch (iSelect_ActorTypeList)
	{
	case 0:
	{
		weak_ptr<CMonster> pObj = Weak_StaticCast<CMonster>(GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL::LEVEL_EDIT));
		pObj.lock()->Set_LinkStateDesc(tMonsterDesc);
		pObj.lock()->Init_Desc();
		pObj.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);
		pObj.lock()->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_PickingDesc));

		Add_ActorToTool(typeid(CNorMonster).hash_code(), typeid(CNorMonster).name(), pObj);
	}
	break;

	case 1:
	{
		weak_ptr<CMonster> pObj;

		if (MONSTERTYPE::JOKER == tMonsterDesc.eMonType)
		{
			pObj = Weak_StaticCast<CMonster>(GAMEINSTANCE->Add_GameObject<CJoker>(LEVEL::LEVEL_EDIT));
			Add_ActorToTool(typeid(CJoker).hash_code(), typeid(CJoker).name(), pObj);
		}
		else if (MONSTERTYPE::BIGHANDMAN == tMonsterDesc.eMonType)
		{
			pObj = Weak_StaticCast<CMonster>(GAMEINSTANCE->Add_GameObject<CBigHandMan>(LEVEL::LEVEL_EDIT));
			Add_ActorToTool(typeid(CBigHandMan).hash_code(), typeid(CBigHandMan).name(), pObj);
		}

	
		if (!pObj.lock())
			return;

		pObj.lock()->Set_LinkStateDesc(tMonsterDesc);
		pObj.lock()->Init_Desc();
		pObj.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);
		pObj.lock()->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_PickingDesc));
	}
	break;

	case 2:
	{
		weak_ptr<CMonster> pObj;

		if (MONSTERTYPE::VARG == tMonsterDesc.eMonType)
		{
			pObj = Weak_StaticCast<CMonster>(GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL::LEVEL_EDIT));
			Add_ActorToTool(typeid(CVarg).hash_code(), typeid(CVarg).name(), pObj);
		}
		else if (MONSTERTYPE::BAT == tMonsterDesc.eMonType)
		{
			pObj = Weak_StaticCast<CMonster>(GAMEINSTANCE->Add_GameObject<CBat>(LEVEL::LEVEL_EDIT));
			Add_ActorToTool(typeid(CBat).hash_code(), typeid(CBat).name(), pObj);
		}
		else if (MONSTERTYPE::URD == tMonsterDesc.eMonType)
		{
			pObj = Weak_StaticCast<CMonster>(GAMEINSTANCE->Add_GameObject<CUrd>(LEVEL::LEVEL_EDIT));
			Add_ActorToTool(typeid(CUrd).hash_code(), typeid(CUrd).name(), pObj);
		}

		if (!pObj.lock())
			return;

		pObj.lock()->Set_LinkStateDesc(tMonsterDesc);
		pObj.lock()->Init_Desc();
		pObj.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);
		pObj.lock()->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_PickingDesc));
	}
	break;

	case 3:
	{
		weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CCorvus>(LEVEL::LEVEL_EDIT);
		pObj.lock()->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_PickingDesc));
		pObj.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);

		Add_ActorToTool(typeid(CCorvus).hash_code(), typeid(CCorvus).name(), pObj);
	}
	break;
	}

}

void CEditSetActor::View_Picking_Actor()
{
	RAY MouseRayInWorldSpace;

	if (!KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) || !Picking(MouseRayInWorldSpace, 0.f))
		return;

	MESH_VTX_INFO VtxInfo;
	VtxInfo.vMin = { -1.f, 0.f, -1.f };
	VtxInfo.vMax = {  1.f, 2.f,  1.f };

	_uint   iIndex = 0;
	_float  fDistance = 99999999.f;
	_float4	vCamPosition = GAMEINSTANCE->Get_CamPosition();
	_vector vCamPos = XMLoadFloat4(&vCamPosition);

	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

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
				fDistance = fLength;
				m_iPickingIndex = iIndex;
				XMStoreFloat4x4(&m_PickingDesc, pTransform.lock()->Get_WorldMatrix());
			}
		}

		++iIndex;
	}

	if (0 > m_iPickingIndex && (_int)iter_collider->second.size() <= m_iPickingIndex)
	{
		m_iPickingIndex = -1;
	}
}

void CEditSetActor::View_Picking_List()
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

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

				string szTag = "( " + to_string(i) + " ) " + iter_collider->second[i].TypeName;

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

void CEditSetActor::View_SelectTransformInfo()
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return;

	if (iter_collider->second.size() <= m_iPickingIndex)
		return;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(iter_collider->second[m_iPickingIndex].pInstance);

	if (pMonster.lock())
	{
		CMonster::STATE_LINK_MONSTER_DESC Desc = pMonster.lock()->Get_LinkStateDesc();

		if (ImGui::Checkbox("Select Desc.bPatrol", &Desc.bPatrol))
			pMonster.lock()->Set_LinkStateDesc(Desc);
	}

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
	/*_float3 vScaleFloat3 = pTransformCom.lock()->Get_Scaled();
	_vector vScaleVector = XMLoadFloat3(&vScaleFloat3);

	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &vScaleVector.m128_f32[0], 0.1f);

	vScaleVector.m128_f32[0] = max(0.00001f, vScaleVector.m128_f32[0]);
	vScaleVector.m128_f32[1] = max(0.00001f, vScaleVector.m128_f32[1]);
	vScaleVector.m128_f32[2] = max(0.00001f, vScaleVector.m128_f32[2]);
	XMStoreFloat3(&vScaleFloat3, vScaleVector);

	pTransformCom.lock()->Set_Scaled(vScaleFloat3);*/
	ImGui::Text("");

	XMStoreFloat4x4(&m_PickingDesc, pTransformCom.lock()->Get_WorldMatrix());
}

void CEditSetActor::View_Picking_Option()
{
	ImGui::Text("");
	ImGui::Text(string("Pick Index : " + to_string(m_iPickingIndex)).c_str());

	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return;

	// Z : 터레인 이동
	// X : 회전
	// C : Y 이동
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

		else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
			{
				weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();
				_matrix matWorld = pTransformCom.lock()->Get_WorldMatrix();
				_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matWorld));

				vPitchYawRoll.y += 0.01f * MouseMove;

				_vector vQuaternion = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

				pTransformCom.lock()->Rotation_Quaternion(vQuaternion);
			}
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
				weak_ptr<CTransform> pTransformCom = iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();
				_vector vPos = pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
				vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + MouseMove * -0.01f);

				pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
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

void    CEditSetActor::View_Picking_MessageEdit()
{
	ImGui::Text("");
	ImGui::Separator();

	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return;

	iter_collider->second[m_iPickingIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW);
}

void CEditSetActor::SetUp_ShaderResource_Select(ID3D11DeviceContext* pDeviceContext)
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_collider->second.empty() || 0 > m_iPickingIndex || iter_collider->second.size() <= m_iPickingIndex)
		return;

	MESH_VTX_INFO VtxInfo;
	VtxInfo.vMin = { -1.f, 0.f, -1.f };
	VtxInfo.vMax = { 1.f, 2.f,  1.f };

	m_pSelect_VIBufferCom.lock()->Update
	(
		VtxInfo,
		iter_collider->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>().lock()->Get_WorldMatrix()
	);

	_float4x4 Matrix;
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());

	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_WorldMatrix", &Matrix, sizeof(_float4x4))))
		return;
	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return;
	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return;

	m_pSelect_ShaderCom.lock()->Begin(1, pDeviceContext);
	m_pSelect_VIBufferCom.lock()->Render(pDeviceContext);
}

void CEditSetActor::Add_ActorToTool(_hashcode _HashCode, string _szTypeName, weak_ptr<CGameObject>_pInstance)
{
	auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditSetActor).hash_code());

	CWindow_HierarchyView::GAMEOBJECT_DESC tObjDesc;

	tObjDesc.HashCode = _HashCode;
	tObjDesc.pInstance = _pInstance;
	tObjDesc.TypeName = _szTypeName;

	if (iter_collider == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
	{
		vector<CWindow_HierarchyView::GAMEOBJECT_DESC> List;
		List.push_back(tObjDesc);

		GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup[typeid(CEditSetActor).hash_code()] = List;

		m_iPickingIndex = 0;
	}
	else
	{
		iter_collider->second.push_back(tObjDesc);

		m_iPickingIndex = (_uint)iter_collider->second.size() - 1;
	}
}

_bool CEditSetActor::Picking(RAY& _pMouseRayInWorldSpace, _float _fSetY)
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return false;

	_pMouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	_float4 vOutPos;
	ZeroMemory(&vOutPos, sizeof(_float4));
	vOutPos.y = _fSetY;

	if (SMath::Is_Picked_AbstractTerrain(_pMouseRayInWorldSpace, &vOutPos))
	{
		vOutPos.y = m_PickingDesc._42;
		memcpy(&m_PickingDesc.m[3], &vOutPos, sizeof(_float3));

		return true;
	}

	return false;
}

void CEditSetActor::Free()
{
}
