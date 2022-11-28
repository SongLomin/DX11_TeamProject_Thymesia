#include "stdafx.h"

#include "EditInstanceProp.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"

#include "Model.h"
#include "Static_Instancing_Prop.h"

#include "Window_PrototypeView.h"
#include "Window_HierarchyView.h"
#include "ImGui_Window.h"

_bool CEditInstanceProp::m_bDetailPicking = true;

GAMECLASS_C(CEditInstanceProp)
CLONE_C(CEditInstanceProp, CGameObject)

HRESULT CEditInstanceProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditInstanceProp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxModelInstance"),
		VTXMODEL_INSTANCE_DECLARATION::Element,
		VTXMODEL_INSTANCE_DECLARATION::iNumElements
	);

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

	m_pSelect_ShaderCom = Add_Component<CShader>();
	m_pSelect_ShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxCubeTex"),
		VTXCUBETEX_DECLARATION::Element,
		VTXCUBETEX_DECLARATION::iNumElements
	);

	m_pInstanceModelCom   = Add_Component<CVIBuffer_Model_Instance>();
	m_pSelect_VIBufferCom = Add_Component<CVIBuffer_DynamicCube>();

	m_ModelList = GET_SINGLE(CGameInstance)->Get_AllNoneAnimModelKeys();

	ZeroMemory(&m_PickingDesc, sizeof(INSTANCE_MESH_DESC));
	m_PickingDesc.vScale = _float3(1.f, 1.f, 1.f);

	Use_Thread(THREAD_TYPE::CUSTOM_THREAD1);

	return S_OK;
}

HRESULT CEditInstanceProp::Start()
{
	__super::Start();

	return S_OK;
}

void CEditInstanceProp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pInstanceModelCom.lock()->Update(m_pPropInfos, true);
}

void CEditInstanceProp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

void CEditInstanceProp::Custom_Thread1(_float fTimeDelta)
{
#ifdef _INSTANCE_CULLING_
	m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos));
#endif
}

void CEditInstanceProp::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);

#ifdef _INSTANCE_CULLING_
	m_pInstanceModelCom.lock()->Update_VisibleInstance();
#endif
}

HRESULT CEditInstanceProp::Render()
{
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

void CEditInstanceProp::Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName)
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

HRESULT CEditInstanceProp::SetUp_ShaderResource()
{
	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_vector vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (m_bNonCulling)
		{
			if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
				m_iPassIndex = 4;
			else
				m_iPassIndex = 5;
		}
		else
		{
			if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
				m_iPassIndex = 0;
			else
				m_iPassIndex = 1;
		}

		if (m_bViewPhysXInfo && m_iColliderType != 0)
		{
			m_iPassIndex = 3;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pInstanceModelCom.lock()->Render_Mesh(i);
	}

	SetUp_ShaderResource_Select();

	return __super::Render();
}

void CEditInstanceProp::SetUp_ShaderResource_Select()
{
	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;
	
	if (!m_bSubDraw)
		return;

	_matrix PickWorldMatrix = XMMatrixIdentity();
	_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_pPropInfos[m_iPickingIndex].vRotation));

	PickWorldMatrix.r[0] = RotationMatrix.r[0] * m_pPropInfos[m_iPickingIndex].vScale.x;
	PickWorldMatrix.r[1] = RotationMatrix.r[1] * m_pPropInfos[m_iPickingIndex].vScale.y;
	PickWorldMatrix.r[2] = RotationMatrix.r[2] * m_pPropInfos[m_iPickingIndex].vScale.z;
	PickWorldMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&m_pPropInfos[m_iPickingIndex].vTarnslation), 1.f);

	m_pSelect_VIBufferCom.lock()->Update
	(
		m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo,
		PickWorldMatrix
	);

	_float4x4 Matrix;
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());

	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_WorldMatrix", &Matrix, sizeof(_float4x4))))
		return;
	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return;
	if (FAILED(m_pSelect_ShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return;

	m_pSelect_ShaderCom.lock()->Begin(1);
	m_pSelect_VIBufferCom.lock()->Render();
}

void CEditInstanceProp::Write_Json(json& Out_Json)
{
	if ("" == m_szSelectModelName)
	{
		MSG_BOX("Check : m_szSelectModelName value is None");
		return;
	}

	json PropInfo;

	_uint iIndex = 0;
	for (auto& iter : m_pPropInfos)
	{
		_float4x4 PropMatrix;
		ZeroMemory(&PropMatrix, sizeof(_float4x4));

		memcpy(&PropMatrix.m[0], &iter.vRotation.x		, sizeof(_float3));
		memcpy(&PropMatrix.m[1], &iter.vScale.x			, sizeof(_float3));
		memcpy(&PropMatrix.m[2], &iter.vTarnslation.x	, sizeof(_float3));

		PropInfo.emplace(string("Prop Matrix (" + to_string(iIndex++) + ")"), PropMatrix.m);
	}

	Out_Json.emplace("PropDesc", PropInfo);
	Out_Json.emplace("ModelCom", m_pInstanceModelCom.lock()->Get_ModelKey());

	if (m_bNonCulling)
		Out_Json.emplace("NonCulling", 1);

	if (Out_Json.end() != Out_Json.find("Hash"))
	{
		Out_Json["Hash"] = typeid(CStatic_Instancing_Prop).hash_code();
	}

	if (Out_Json.end() != Out_Json.find("Name"))
	{
		Out_Json["Name"] = typeid(CStatic_Instancing_Prop).name();
	}

	Out_Json.emplace("Collider_Type", m_iColliderType);
}

void CEditInstanceProp::Load_FromJson(const json& In_Json)
{
	for (auto& iter : In_Json.items())
	{
		string szKey = iter.key();

		if ("NonCulling" == szKey)
		{
			m_bNonCulling = (1 == iter.value()) ? (true) : (false);
		}

		else if ("ModelCom" == szKey)
		{
			m_szSelectModelName = iter.value();
			m_pInstanceModelCom.lock()->Init_Model(m_szSelectModelName.c_str());
		}

		else if ("PropDesc" == szKey)
		{
			json Desc = iter.value();

			_vector vPosition;
			_vector vOffsetRange;

			for (auto& iter_item : Desc.items())
			{
				_float4x4 PropMatrix;

				CJson_Utility::Load_JsonFloat4x4(iter_item.value(), PropMatrix);

				INSTANCE_MESH_DESC	Desc;
				ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

				memcpy(&Desc.vRotation   , &PropMatrix.m[0][0], sizeof(_float3));
				memcpy(&Desc.vScale      , &PropMatrix.m[1][0], sizeof(_float3));
				memcpy(&Desc.vTarnslation, &PropMatrix.m[2][0], sizeof(_float3));

				vPosition = XMLoadFloat3(&Desc.vTarnslation);
				vPosition = XMVectorSetW(vPosition, 1.f);
				
				MESH_VTX_INFO tInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

				vOffsetRange = XMLoadFloat3(&tInfo.vMax) - XMLoadFloat3(&tInfo.vMin);
				vOffsetRange *= XMLoadFloat3(&Desc.vScale);
				Desc.fMaxRange = XMVectorGetX(XMVector3Length(vOffsetRange));
				Desc.vCenter = tInfo.vCenter;
				Desc.Bake_CenterWithMatrix();

				m_pPropInfos.push_back(Desc);
			}
		}
	}

	m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());

	if (In_Json.end() != In_Json.find("Collider_Type"))
	{
		m_iColliderType = In_Json["Collider_Type"];
	}
}

_bool CEditInstanceProp::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	_float fPickedDist;
	_bool bPicked = false;
	_uint iIndex = 0;

	for (auto& iter : m_pPropInfos)
	{
		if (!m_pInstanceModelCom.lock().get())
		{
			return false;
		}

		MESH_VTX_INFO Info = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		_matrix PickWorldMatrix = iter.Get_Matrix();

		if (SMath::Is_Picked_AbstractCube(In_Ray, Info, PickWorldMatrix, &fPickedDist))
		{
			if (Out_fRange > fPickedDist)
			{
				m_PickingDesc = iter;
				m_iPickingIndex = iIndex;
				bPicked = true;
				Out_fRange = fPickedDist;
			}
		}

		++iIndex;
	}

	return bPicked;
}

void CEditInstanceProp::OnEventMessage(_uint iArg)
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

		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			if (!m_bSubDraw)
				return;

			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Create"))
				{

					View_PhysXOption();
					View_SelectModelComponent();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Pick"))
				{
					View_PickingInfo();

					View_Picking_Prop();
					View_Picking_List();
					View_Picking_Option();
					View_SelectTransformInfo();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Save & Load"))
				{
					View_SelectJson();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO:
		{
			m_bViewPhysXInfo = true;
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO_N:
		{
			m_bViewPhysXInfo = false;
		}
		break;
	}
}

void CEditInstanceProp::View_SelectModelComponent()
{
	static _int		iSelect_NonAnimModel	 = 0;
	static _char    szFindModelTag[MAX_PATH] = "";
	static _uint    iPropMaxSzie = 0;

	ImGui::Text(string("[ Select MD ] : " + m_szSelectModelName).c_str());
	ImGui::InputText("Find", szFindModelTag, MAX_PATH);

	if (ImGui::BeginListBox("##Model", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (_uint i = 0; i < m_ModelList.size(); ++i)
		{
			const bool is_selected = (iSelect_NonAnimModel == iPropMaxSzie);

			if (0 < strlen(szFindModelTag))
			{
				if (string::npos == m_ModelList[i].find(szFindModelTag))
					continue;
			}

			if (ImGui::Selectable(m_ModelList[i].c_str(), is_selected))
			{
				iSelect_NonAnimModel = i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Select", ImVec2(100.f, 25.f)))
	{
		m_szSelectModelName = m_ModelList[iSelect_NonAnimModel];
		m_pInstanceModelCom.lock()->Init_Model(m_szSelectModelName.c_str());
	}

	ImGui::Checkbox("NinCulling", &m_bNonCulling);

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_PickingInfo()
{
	ImGui::Text("");

	if (!m_pPropInfos.empty() && 0 <= m_iPickingIndex && (_uint)m_pPropInfos.size() > m_iPickingIndex)
	{
		m_PickingDesc.vTarnslation.y = m_pPropInfos[m_iPickingIndex].vTarnslation.y;
	}

	ImGui::DragFloat3("PickPos" , &m_PickingDesc.vTarnslation.x, 1.f);


	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_Picking_Prop()
{
	ImGui::Checkbox("Detail", &m_bDetailPicking);

	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return;
	
	// 생성
	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if ("" == m_szSelectModelName)
			return;

		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		_float4 vOutPos;
		ZeroMemory(&vOutPos, sizeof(_float4));
		vOutPos.y = m_PickingDesc.vTarnslation.y;

		if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vOutPos))
			return;

		vOutPos.y = m_PickingDesc.vTarnslation.y;
		memcpy(&m_PickingDesc.vTarnslation, &vOutPos, sizeof(_float3));

		m_pPropInfos.push_back(m_PickingDesc);
		m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		m_iPickingIndex = (_uint)m_pPropInfos.size() - 1;
	}

	// 피킹
	else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
	{			
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		if (!m_pInstanceModelCom.lock()->Get_ModelData().lock())
			return;

		MESH_VTX_INFO VtxInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		_uint   iIndex    = 0;
		_float  fDistance = 99999999.f;
		_float4	vCamPosition = GAMEINSTANCE->Get_CamPosition();
		_vector vCamPos   = XMLoadFloat4(&vCamPosition);

		for (auto& iter : m_pPropInfos)
		{
			_matrix WorlMatrix     = XMMatrixIdentity();
			_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&iter.vRotation));

			WorlMatrix.r[0] = RotationMatrix.r[0] * iter.vScale.x;
			WorlMatrix.r[1] = RotationMatrix.r[1] * iter.vScale.y;
			WorlMatrix.r[2] = RotationMatrix.r[2] * iter.vScale.z;
			WorlMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&iter.vTarnslation), 1.f);

			if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(WorlMatrix.r[3], iter.vScale.x))
			{
				if (m_bDetailPicking)
				{
					if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, WorlMatrix))
					{
						_float  fLength = XMVectorGetX(XMVector3Length(vCamPos - XMLoadFloat3(&iter.vTarnslation)));

						if (fLength < fDistance)
						{
							fDistance       = fLength;
							m_iPickingIndex = iIndex;
							m_PickingDesc   = m_pPropInfos[iIndex];
						}
					}
				}
				else
				{
					if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, VtxInfo, WorlMatrix))
					{
						m_iPickingIndex = iIndex;
						m_PickingDesc   = m_pPropInfos[iIndex];

						break;
					}
				}
			}

			++iIndex;
		}
	}
}

void CEditInstanceProp::View_Picking_List()
{
	ImGui::Text(string(string(" Size : ") + to_string((_uint)m_pPropInfos.size())).c_str());

	if (ImGui::TreeNode("[ Show List ]"))
	{
		if (ImGui::BeginListBox("##Prop Info List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (_uint i = 0; i < (_uint)m_pPropInfos.size(); ++i)
			{
				const bool is_selected = (m_iPickingIndex == i);

				string szTag = "( " + to_string(i) + " ) " + m_szSelectModelName;

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

	if (ImGui::Button("Create", ImVec2(100.f, 25.f)) || KEY_INPUT(KEY::M, KEY_STATE::TAP))
	{
		if ("" == m_szSelectModelName)
			return;

		m_pPropInfos.push_back(m_PickingDesc);
		m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		m_iPickingIndex = (_uint)m_pPropInfos.size() - 1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete", ImVec2(100.f, 25.f)))
	{
		if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
			return;

		auto iter = m_pPropInfos.begin() + m_iPickingIndex;

		if (m_pPropInfos.end() != iter)
		{
			m_pPropInfos.erase(iter);
			m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		}
	}
}

void CEditInstanceProp::View_Picking_Option()
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

	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;

	// Z : 터레인 이동
	// X : 회전(옵션 활성화)
	// C : 수동 이동(옵션 활성화)
	// R : 삭제

	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD))
	{
		RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

		_float4 vMouseDir;

		if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		{
			vMouseDir.y = m_PickingDesc.vTarnslation.y;

			if (!SMath::Is_Picked_AbstractTerrain(MouseRayInWorldSpace, &vMouseDir))
				return;

			_float3 vObjPos = m_pPropInfos[m_iPickingIndex].vTarnslation;
			m_pPropInfos[m_iPickingIndex].vTarnslation = _float3(vMouseDir.x, vObjPos.y, vMouseDir.z);
		}

		else if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
		{
			_long		MouseMove = 0;
			if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
			{
				switch (m_iOption)
				{
				case 0: m_pPropInfos[m_iPickingIndex].vRotation.x += 0.01f * MouseMove; break;
				case 1: m_pPropInfos[m_iPickingIndex].vRotation.y += 0.01f * MouseMove; break;
				case 2: m_pPropInfos[m_iPickingIndex].vRotation.z += 0.01f * MouseMove; break;
				}
			}
		}

		else if (KEY_INPUT(KEY::C, KEY_STATE::HOLD))
		{
			switch (m_iOption)
			{
			case 0:
			case 2:
			{
				_long		MouseMoveX = 0;
				if (MouseMoveX = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
				{
					m_pPropInfos[m_iPickingIndex].vTarnslation.x += 0.01f * MouseMoveX;
				}

				_long		MouseMoveZ = 0;
				if (MouseMoveZ = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
				{
					m_pPropInfos[m_iPickingIndex].vTarnslation.z -= 0.01f * MouseMoveZ;
				}
			}
			break;

			case 1:
			{
				_long		MouseMove = 0;
				if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
				{
					m_pPropInfos[m_iPickingIndex].vTarnslation.y -= 0.01f * MouseMove;
				}
			}
			break;
			}
		}

		else if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
		{
			switch (m_iOption)
			{
			case 0:
			{
				if (KEY_INPUT(KEY::SPACE, KEY_STATE::HOLD))
					m_pPropInfos[m_iPickingIndex].vScale.x -= 0.1f;
				else
					m_pPropInfos[m_iPickingIndex].vScale.x += 0.1f;
			}
			break;

			case 1:
			{
				if (KEY_INPUT(KEY::SPACE, KEY_STATE::HOLD))
					m_pPropInfos[m_iPickingIndex].vScale.y -= 0.1f;
				else
					m_pPropInfos[m_iPickingIndex].vScale.y += 0.1f;
			}
			break;

			case 2:
			{
				if (KEY_INPUT(KEY::SPACE, KEY_STATE::HOLD))
					m_pPropInfos[m_iPickingIndex].vScale.z -= 0.1f;
				else
					m_pPropInfos[m_iPickingIndex].vScale.z += 0.1f;
			}
			break;

			}
		}

		else if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
		{
			switch (m_iOption)
			{
				case 0:
				{
					if (KEY_INPUT(KEY::SPACE, KEY_STATE::HOLD))
						m_pPropInfos[m_iPickingIndex].vScale.x -= 0.1f;
					else
						m_pPropInfos[m_iPickingIndex].vScale.x += 0.1f;
				}
				break;

				case 1:
				{
					if (KEY_INPUT(KEY::SPACE, KEY_STATE::HOLD))
						m_pPropInfos[m_iPickingIndex].vScale.y -= 0.1f;
					else
						m_pPropInfos[m_iPickingIndex].vScale.y += 0.1f;
				}
				break;

				case 2:
				{
					if (KEY_INPUT(KEY::SPACE, KEY_STATE::HOLD))
						m_pPropInfos[m_iPickingIndex].vScale.z -= 0.1f;
					else
						m_pPropInfos[m_iPickingIndex].vScale.z += 0.1f;
				}
				break;

			}
		}
	}

	else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::R, KEY_STATE::TAP))
	{
		if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
			return;

		auto iter = m_pPropInfos.begin() + m_iPickingIndex;

		if (m_pPropInfos.end() != iter)
		{
			m_pPropInfos.erase(iter);
			m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
		}
	}
}

void CEditInstanceProp::View_SelectTransformInfo()
{
	ImGui::Separator();

	if (m_pPropInfos.empty() || 0 > m_iPickingIndex || m_pPropInfos.size() <= m_iPickingIndex)
		return;

	// Position
	ImGui::Text("Position");
	ImGui::DragFloat3("##Position", &m_pPropInfos[m_iPickingIndex].vTarnslation.x, 1.f);
	
	 // Quaternion
	ImGui::Text("Pitch Yaw Roll");
	ImGui::DragFloat3("##Pitch Yaw Roll", &m_pPropInfos[m_iPickingIndex].vRotation.x, 0.01f);

	 // Scale
	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &m_pPropInfos[m_iPickingIndex].vScale.x, 0.1f);

	ImGui::Text("");
	ImGui::Separator();
}

void CEditInstanceProp::View_SelectJson()
{
	static _char szJsonFileTag[MAX_PATH] = "";
	ImGui::InputText("Name", szJsonFileTag, MAX_PATH);

	static int	iSelect_MeshData = 0;
	if (ImGui::BeginListBox("##Json List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		_uint iIndex = 0;

		for (auto& iter : m_JsonList)
		{
			const bool is_selected = (iSelect_MeshData == iIndex);

			if (0 < strlen(szJsonFileTag))
			{
				if (string::npos == m_JsonList[iIndex].find(szJsonFileTag))
					continue;
			}

			if (ImGui::Selectable(iter.c_str(), is_selected))
			{
				iSelect_MeshData = iIndex;
				strcpy_s(szJsonFileTag, m_JsonList[iIndex].c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}
}

void CEditInstanceProp::View_PhysXOption()
{
	const _char* items[] = { "None", "Model", "ConvexModel" };

	if (ImGui::BeginListBox("PhysX Collider Type"))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (m_iColliderType == n);
			if (ImGui::Selectable(items[n], is_selected))
			{
				m_iColliderType = n;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CEditInstanceProp::Free()
{
}