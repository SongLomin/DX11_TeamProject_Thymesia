#include "stdafx.h"

#include "EditGroupProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Collider.h"

#include "Client_GameObjects.h"

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

	XMStoreFloat4x4(&m_PickingMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CEditGroupProp::Start()
{
	return S_OK;
}

void CEditGroupProp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEditGroupProp::LateTick(_float fTimeDelta)
{
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
	if (!m_bSubDraw)
		return S_OK;

	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return E_FAIL;

	if (iter_prop->second.empty() || 0 > m_iPickingIndex || iter_prop->second.size() <= m_iPickingIndex)
		return E_FAIL;

	weak_ptr<CTransform>	pTransform = iter_prop->second[m_iPickingIndex].pInstance.lock()->Get_Component<CTransform>();
	weak_ptr<CModel>		pModel     = iter_prop->second[m_iPickingIndex].pInstance.lock()->Get_Component<CModel>();

	if (!pModel.lock())
	{
		COLLIDERDESC CollDesc = iter_prop->second[m_iPickingIndex].pInstance.lock()->Get_Component<CCollider>().lock()->Get_ColliderDesc();

		MESH_VTX_INFO	VtxInfo;
		VtxInfo.vMin = { CollDesc.vScale.x * -0.5f, CollDesc.vScale.x * -0.5f, CollDesc.vScale.x * -0.5f };
		VtxInfo.vMax = { CollDesc.vScale.x *  0.5f, CollDesc.vScale.x *  0.5f, CollDesc.vScale.x *  0.5f };

		m_pVIBufferCom.lock()->Update
		(
			VtxInfo,
			pTransform.lock()->Get_WorldMatrix()
		);
	}
	else
	{
		m_pVIBufferCom.lock()->Update
		(
			pModel.lock()->Get_MeshVertexInfo(),
			pTransform.lock()->Get_WorldMatrix()
		);
	}

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

void CEditGroupProp::Write_Json(json& Out_Json)
{
}

_bool CEditGroupProp::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return false;

	_float fPickedDist;
	_bool bPicked = false;
	_uint iIndex  = 0;

	for (auto& iter : iter_prop->second)
	{
		weak_ptr<CModel>		pModelCom     = iter.pInstance.lock()->Get_Component<CModel>();
		weak_ptr<CTransform>	pTransformCom = iter.pInstance.lock()->Get_Component<CTransform>();

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
					View_CreateProp();
					View_PickProp();
					View_OnlyTranformEdit();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Edit"))
				{
					View_PickProp();
					View_EditProp();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;

		case EVENT_TYPE::ON_EDIT_DELETE:
		{

			auto iter_collider = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

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

void CEditGroupProp::View_CreateProp()
{
	ImGui::Text("");

	static const char* items_PropType[] =
	{
		"Interaction_Prop",
		"Static_Prop",
		"Dynamic_Prop",
		"Light_Prop",
		"Section_Eventer"
	};

	static const char* items_Interaction_Prop[] =
	{
		"Door",
		"CheckPoint",
		"Elevator",
		"Ladder",
		"Note"
	};

	static _int iSelect_PropType = 0;
	static _int iSelect_PropName = 0;

	if (ImGui::Combo("Prop Type", &iSelect_PropType, items_PropType, IM_ARRAYSIZE(items_PropType)))
	{
		iSelect_PropName = 0;
	}

	RAY MouseRayInWorldSpace;
	CWindow_HierarchyView::GAMEOBJECT_DESC tObjDesc;

	if (0 == iSelect_PropType)
	{
		ImGui::Combo("Prop Name", &iSelect_PropName, items_Interaction_Prop, IM_ARRAYSIZE(items_Interaction_Prop));

		switch (iSelect_PropName)
		{
			case 0:
			{
				if (RenderView_SelectModelComponent())
					return;

				if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
					return;

				tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CInteraction_Door>(LEVEL::LEVEL_EDIT);
				tObjDesc.HashCode  = typeid(CInteraction_Door).hash_code();
				tObjDesc.TypeName  = typeid(CInteraction_Door).name();

				tObjDesc.pInstance.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str());
			}
			break;

			case 1:
			{
				if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
					return;

				tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CInteraction_CheckPoint>(LEVEL::LEVEL_EDIT);
				tObjDesc.HashCode  = typeid(CInteraction_CheckPoint).hash_code();
				tObjDesc.TypeName  = typeid(CInteraction_CheckPoint).name();
			}
			break;

			case 2:
			{
				if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
					return;

				tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CInteraction_Elevator>(LEVEL::LEVEL_EDIT);
				tObjDesc.HashCode  = typeid(CInteraction_Elevator).hash_code();
				tObjDesc.TypeName  = typeid(CInteraction_Elevator).name();
			}
			break;

			case 3:
			{
				if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
					return;

				tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CInteraction_Ladder>(LEVEL::LEVEL_EDIT);
				tObjDesc.HashCode  = typeid(CInteraction_Ladder).hash_code();
				tObjDesc.TypeName  = typeid(CInteraction_Ladder).name();
			}
			break;
			
			case 4:
			{
				if (RenderView_SelectModelComponent())
					return;

				if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
					return;

				tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CInteraction_Note>(LEVEL::LEVEL_EDIT);
				tObjDesc.HashCode  = typeid(CInteraction_Note).hash_code();
				tObjDesc.TypeName  = typeid(CInteraction_Note).name();

				tObjDesc.pInstance.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str());
			}
			break;
		}
	}

	else if (1 == iSelect_PropType)
	{
		if (RenderView_SelectModelComponent())
			return;

		if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
			return;

		tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CStatic_Prop>(LEVEL::LEVEL_EDIT);
		tObjDesc.HashCode  = typeid(CStatic_Prop).hash_code();
		tObjDesc.TypeName  = typeid(CStatic_Prop).name();

		tObjDesc.pInstance.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str());
	}

	else if (2 == iSelect_PropType)
	{
		if (RenderView_SelectModelComponent())
			return;

		if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
			return;

		tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(LEVEL::LEVEL_EDIT);
		tObjDesc.HashCode = typeid(CDynamic_Prop).hash_code();
		tObjDesc.TypeName = typeid(CDynamic_Prop).name();

		tObjDesc.pInstance.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str());
	}

	else if (3 == iSelect_PropType)
	{
		if (RenderView_SelectModelComponent())
			return;

		if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
			return;

		tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL::LEVEL_EDIT);
		tObjDesc.HashCode  = typeid(CLight_Prop).hash_code();
		tObjDesc.TypeName  = typeid(CLight_Prop).name();

		tObjDesc.pInstance.lock()->Get_Component<CModel>().lock()->Init_Model(m_szSelectModelName.c_str());
	}

	else if (4 == iSelect_PropType)
	{
		if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) || !Pick_Prop(MouseRayInWorldSpace))
			return;

		tObjDesc.pInstance = GAMEINSTANCE->Add_GameObject<CSection_Eventer>(LEVEL::LEVEL_EDIT);
		tObjDesc.HashCode  = typeid(CSection_Eventer).hash_code();
		tObjDesc.TypeName  = typeid(CSection_Eventer).name();
	}

	else
	{
		return;
	}

	if (!tObjDesc.pInstance.lock())
		return;

	tObjDesc.pInstance.lock()->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_PickingMatrix));
	tObjDesc.pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITINIT);

	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
	{
		vector<CWindow_HierarchyView::GAMEOBJECT_DESC> List;
		List.push_back(tObjDesc);
		
		GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup[typeid(CEditGroupProp).hash_code()] = List;
		m_iPickingIndex = 0;
	}
	else
	{
		iter_prop->second.push_back(tObjDesc);
		m_iPickingIndex = (_int)iter_prop->second.size() - 1;
	}

	ImGui::Text("");
	ImGui::Separator();
}

void    CEditGroupProp::View_OnlyTranformEdit()
{
	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_prop->second.empty() || 0 > m_iPickingIndex || iter_prop->second.size() <= m_iPickingIndex)
		return;

	RenderView_Transform_Edit(iter_prop->second[m_iPickingIndex].pInstance);
	iter_prop->second[m_iPickingIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDIT_UDATE);
}

void    CEditGroupProp::View_PickProp()
{
	if (!KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) || !KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return;

	_uint   iIndex       = 0;
	_float  fDistance    = 99999999.f;
	_float4	vCamPosition = GAMEINSTANCE->Get_CamPosition();
	_vector vCamPos      = XMLoadFloat4(&vCamPosition);

	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	for (auto& iter : iter_prop->second)
	{
		weak_ptr<CTransform> pTransform = iter.pInstance.lock()->Get_Component<CTransform>();
		weak_ptr<CModel>     pModel     = iter.pInstance.lock()->Get_Component<CModel>();

		if (pModel.lock())
		{
			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, pModel.lock()->Get_MeshVertexInfo(), pTransform.lock()->Get_WorldMatrix()))
			{
				_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION)));

				if (fLength < fDistance)
				{
					fDistance       = fLength;
					m_iPickingIndex = iIndex;
					XMStoreFloat4x4(&m_PickingMatrix, pTransform.lock()->Get_WorldMatrix());
				}
			}
		}
		else
		{
			if (typeid(CSection_Eventer).hash_code() != iter.HashCode)
			{
				++iIndex;
				continue;
			}

			COLLIDERDESC CollDesc = iter.pInstance.lock()->Get_Component<CCollider>().lock()->Get_ColliderDesc();

			MESH_VTX_INFO	VtxInfo;
			VtxInfo.vMin = { CollDesc.vScale.x * -0.5f, 0.f              , CollDesc.vScale.x * -0.5f };
			VtxInfo.vMax = { CollDesc.vScale.x *  0.5f, CollDesc.vScale.x, CollDesc.vScale.x *  0.5f };

			if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, pTransform.lock()->Get_WorldMatrix()))
			{
				_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION)));

				if (fLength < fDistance)
				{
					fDistance       = fLength;
					m_iPickingIndex = iIndex;
					XMStoreFloat4x4(&m_PickingMatrix, pTransform.lock()->Get_WorldMatrix());
				}
			}
		}

		++iIndex;
	}

	if (0 > m_iPickingIndex && (_int)iter_prop->second.size() <= m_iPickingIndex)
	{
		m_iPickingIndex = 0;
	}
}

_bool CEditGroupProp::RenderView_SelectModelComponent()
{
	ImGui::Text("");
	ImGui::Separator();

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
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}

	if (m_bChangModel = ImGui::Button("Chage Model", ImVec2(100.f, 25.f)))
	{
		m_szSelectModelName = m_ModelList[iSelect_NonAnimModel];	
	}

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Separator();

	return ("" == m_szSelectModelName);
}

void CEditGroupProp::View_EditProp()
{
	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return;

	if (iter_prop->second.empty() || 0 > m_iPickingIndex || iter_prop->second.size() <= m_iPickingIndex)
		return;

	ImGui::Text(string(string(" Size  : ") + to_string((_uint)iter_prop->second.size())).c_str());
	ImGui::Text(string(string(" Index : ") + to_string(m_iPickingIndex)).c_str());

	if (ImGui::TreeNode("[ Show List ]"))
	{
		if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (_uint i = 0; i < (_uint)iter_prop->second.size(); ++i)
			{
				const bool is_selected = (m_iPickingIndex == i);

				string szTag = "( " + to_string(i) + " )  " + iter_prop->second[i].TypeName;

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

	ImGui::Text("");
	ImGui::Separator();

	if (typeid(CInteraction_Door).hash_code() == iter_prop->second[m_iPickingIndex].HashCode ||
		typeid(CStatic_Prop).hash_code()      == iter_prop->second[m_iPickingIndex].HashCode ||
		typeid(CLight_Prop).hash_code()       == iter_prop->second[m_iPickingIndex].HashCode)
	{
		weak_ptr<CModel> pModel = iter_prop->second[m_iPickingIndex].pInstance.lock()->Get_Component<CModel>();
		string szModelKeyName = pModel.lock()->Get_ModelKey();

		ImGui::Text(string(string("ModelCom : " + szModelKeyName)).c_str());

		if (!RenderView_SelectModelComponent() && m_bChangModel)
		{
			if (szModelKeyName != m_szSelectModelName)
			{
				pModel.lock()->Init_Model(m_szSelectModelName.c_str());
			}
		}

		ImGui::Text("");
		ImGui::Separator();
	}

	iter_prop->second[m_iPickingIndex].pInstance.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW);

	RenderView_Transform_Info(iter_prop->second[m_iPickingIndex].pInstance);
	RenderView_Transform_Edit(iter_prop->second[m_iPickingIndex].pInstance);
	
	ImGui::Text("");
	ImGui::Separator();
}

void CEditGroupProp::RenderView_Transform_Info(weak_ptr<CGameObject> In_Obj)
{
	weak_ptr<CTransform> pTransformCom = In_Obj.lock()->Get_Transform();

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

void CEditGroupProp::RenderView_Transform_Edit(weak_ptr<CGameObject> In_Obj)
{
	weak_ptr<CTransform> pTransformCom = In_Obj.lock()->Get_Transform();

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);
			_float4 vMouseDir;

			vMouseDir.y = m_PickingMatrix._42;

			if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vMouseDir))
				return;

			pTransformCom.lock()->Set_State
			(
				CTransform::STATE_TRANSLATION,
				XMVectorSet(vMouseDir.x, m_PickingMatrix._42, vMouseDir.z, 1.f)
			);
		}

		else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
			{
				_matrix matWorld      = pTransformCom.lock()->Get_WorldMatrix();
				_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matWorld));

				vPitchYawRoll.y += 0.01f * MouseMove;

				pTransformCom.lock()->Rotation_PitchYawRoll(vPitchYawRoll);
			}
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
			{
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
			auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditGroupProp).hash_code());

			auto iter = iter_prop->second.begin() + m_iPickingIndex;

			if (iter_prop->second.end() != iter)
			{
				iter->pInstance.lock()->Set_Dead();
				iter_prop->second.erase(iter);

				m_iPickingIndex = --m_iPickingIndex;
			}
		}

		else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::H, KEY_STATE::HOLD))
		{
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

	XMStoreFloat4x4(&m_PickingMatrix, pTransformCom.lock()->Get_WorldMatrix());
}

_bool CEditGroupProp::Pick_Prop(RAY& _pMouseRayInWorldSpace)
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return false;

	_pMouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	_float4 vOutPos;
	ZeroMemory(&vOutPos, sizeof(_float4));
	vOutPos.y = m_PickingMatrix._42;

	if (SMath::Is_Picked_AbstractTerrain(_pMouseRayInWorldSpace, &vOutPos))
	{
		vOutPos.y = m_PickingMatrix._42;
		memcpy(&m_PickingMatrix.m[3], &vOutPos, sizeof(_float3));

		return true;
	}

	return false;
}

void CEditGroupProp::Free()
{
}