#include "stdafx.h"
#include "Light_Prop.h"
#include "VIBuffer_Cube.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "GameInstance.h"
#include "GameManager.h"
#include "Model.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"

GAMECLASS_C(CLight_Prop)
CLONE_C(CLight_Prop, CGameObject)

HRESULT CLight_Prop::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLight_Prop::Initialize(void* pArg)
{
	try
	{
		const std::exception Fail;

		if (FAILED(__super::Initialize(pArg)))
			throw Fail;

		m_pModelCom.lock()->Init_Model("Torch", "", (_uint)TIMESCALE_LAYER::NONE);

		m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxModel"),
			VTXMODEL_DECLARATION::Element,
			VTXMODEL_DECLARATION::iNumElements);

		// TODO : need to be data
		m_iPassIndex = 3; // if Normal Map exists, Pass is 3(Normal). else, Pass is 0(Default).

		// TODO : need to be data
		m_pTransformCom.lock()->Set_Position(_fvector{3.f, 15.f, 3.f, 1.f});
		m_pTransformCom.lock()->Set_Scaled(_float3{ 0.8f, 0.8f, 0.8f });

		GET_SINGLE(CGameManager)->Use_EffectGroup("TorchFire", m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));
	}
	catch (const std::exception&)
	{
		assert(0);
		return E_FAIL;
	}

	m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;

	// TODO : need to be data
	ZeroMemory(&m_tLightDesc, sizeof(LIGHTDESC));
	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_HALFPOINT;
	m_tLightDesc.bEnable = true;
	XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_Position());
	m_tLightDesc.vDiffuse = { 1.f, 1.f, 0.8f, 0.f };
	m_tLightDesc.vAmbient = { 0.6f, 0.6f, 0.48f, 0.f };
	m_tLightDesc.vDirection = { 1.f, 0.f, 0.f, 0.f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.fRange = 17.f;

	m_iLightIndex = GAMEINSTANCE->Add_Light(m_tLightDesc);

	PHYSXCOLLIDERDESC tPhysxColliderDesc;
	Preset::PhysXColliderDesc::TestLightPropSetting(tPhysxColliderDesc);

	m_pPhysXColliderCom = Add_Component<CPhysXCollider>(&tPhysxColliderDesc);
	m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene({ 10.f, 10.f, 10.f });

	return S_OK;
}

HRESULT CLight_Prop::Start()
{
	return S_OK;
}

void CLight_Prop::Tick(_float fTimeDelta)
{
	m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom);

	__super::Tick(fTimeDelta);


}

void CLight_Prop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

HRESULT CLight_Prop::Render()
{
	__super::SetUp_ShaderResource();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);
		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pModelCom.lock()->Render_Mesh(i);
	}

	CGameObject::Render();
	return S_OK;
}

void CLight_Prop::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json.emplace("Light_Type", (_int)m_tLightDesc.eActorType);
	Out_Json.emplace("Light_Range", m_tLightDesc.fRange);
	
	CJson_Utility::Write_Float4(Out_Json["Light_Position"], m_tLightDesc.vPosition);
	CJson_Utility::Write_Float4(Out_Json["Light_Direction"], m_tLightDesc.vDirection);
	CJson_Utility::Write_Float4(Out_Json["Light_Diffuse"], m_tLightDesc.vDiffuse);
	CJson_Utility::Write_Float4(Out_Json["Light_Ambient"], m_tLightDesc.vAmbient);
	CJson_Utility::Write_Float4(Out_Json["Light_Specular"], m_tLightDesc.vSpecular);
	CJson_Utility::Write_Float4(Out_Json["Light_Flag"], m_tLightDesc.vLightFlag);

}

void CLight_Prop::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	_int iLightTypeFromInt = In_Json["Light_Type"];
	m_tLightDesc.eActorType = (LIGHTDESC::TYPE)iLightTypeFromInt;
	m_tLightDesc.fRange = In_Json["Light_Range"];

	CJson_Utility::Load_Float4(In_Json["Light_Position"], m_tLightDesc.vPosition);
	CJson_Utility::Load_Float4(In_Json["Light_Direction"], m_tLightDesc.vDirection);
	CJson_Utility::Load_Float4(In_Json["Light_Diffuse"], m_tLightDesc.vDiffuse);
	CJson_Utility::Load_Float4(In_Json["Light_Ambient"], m_tLightDesc.vAmbient);
	CJson_Utility::Load_Float4(In_Json["Light_Specular"], m_tLightDesc.vSpecular);
	CJson_Utility::Load_Float4(In_Json["Light_Flag"], m_tLightDesc.vLightFlag);

	GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
}

void CLight_Prop::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Light_Prop GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			const char* LightTypeItems[] = { "Direction", "Point", "HalfPoint" };

			if (ImGui::BeginListBox("Light Type"))
			{
				for (_int n = 0; n < (_int)(IM_ARRAYSIZE(LightTypeItems)); n++)
				{
					const bool is_selected = ((_int)m_tLightDesc.eActorType == n);
					if (ImGui::Selectable(LightTypeItems[n], is_selected))
					{
						m_tLightDesc.eActorType = (LIGHTDESC::TYPE)n;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			

			ImGui::DragFloat3("Light_Position", &m_tLightDesc.vPosition.x, 0.01f);
			ImGui::DragFloat3("Light_Direction", &m_tLightDesc.vDirection.x, 0.01f);
			ImGui::DragFloat3("Light_Diffuse", &m_tLightDesc.vDiffuse.x, 0.01f);
			ImGui::DragFloat3("Light_Ambient", &m_tLightDesc.vAmbient.x, 0.01f);
			ImGui::DragFloat3("Light_Specular", &m_tLightDesc.vSpecular.x, 0.01f);
			ImGui::DragFloat4("Light_Flag", &m_tLightDesc.vLightFlag.x, 0.01f);
			ImGui::DragFloat("Light_Range", &m_tLightDesc.fRange, 0.01f);

			GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
		}
	}

}

void CLight_Prop::Free()
{
	__super::Free();
	GAMEINSTANCE->Remove_Light(m_tLightDesc.Get_LightIndex());
}